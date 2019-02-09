// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/graphics/RenderGraph.hpp>

#include <storm/core/Strings.hpp>

#include <fstream>
#include <stack>
#include <functional>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
RenderGraph::RenderGraph(const Device &device)
	: m_device{device}, m_next_task_id{1} {

}

/////////////////////////////////////
/////////////////////////////////////
RenderGraph::~RenderGraph() = default;

/////////////////////////////////////
/////////////////////////////////////
RenderGraph::RenderGraph(RenderGraph &&) = default;

/////////////////////////////////////
/////////////////////////////////////
RenderGraph &RenderGraph::operator=(RenderGraph &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void RenderGraph::compile() {
	/*
	 * Compute initial resource and pass reference counts
	 *     renderPass.refCount++ for every resource write
	 *     resource.refCount++ for every resource read
	 * Identify resources with refCount== 0 and push them on a stack
	 *     While stack is non-empty
	 *     Pop a resource and decrement ref count of its producer
	 *     If producer.refCount== 0, decrement ref counts of resources that it reads
	 *          Add them to the stack when their refCount== 0
	 */

	const auto get_resource_func = [this](auto id) -> ResourceBase& {return m_resources.getResource(id);};
	const auto get_render_task_func = [this](auto id) -> RenderTaskBase & {return getRenderTask(id);};

	for(auto &render_task : m_render_tasks)
		render_task->m_ref_count = std::size(render_task->m_create_attachments) + std::size(render_task->m_write_attachments)
								 + std::size(render_task->m_create_buffers) + std::size(render_task->m_write_buffers);

	const auto updateRefs = [](auto &resource){resource->m_ref_count = std::size(resource->m_readers);};
	std::for_each(std::begin(m_resources.textures()), std::end(m_resources.textures()), updateRefs);
	std::for_each(std::begin(m_resources.buffers()), std::end(m_resources.buffers()), updateRefs);

	auto unreferenced_resources = std::stack<std::reference_wrapper<ResourceBase>>{};

	const auto initUnreferencedResources = [&unreferenced_resources](const auto &resource) {
		if(resource->m_ref_count == 0 && resource->transient())
			unreferenced_resources.emplace(*resource);
	};

	std::for_each(std::cbegin(m_resources.textures()), std::cend(m_resources.textures()), initUnreferencedResources);
	std::for_each(std::cbegin(m_resources.buffers()), std::cend(m_resources.buffers()), initUnreferencedResources);

	while(!unreferenced_resources.empty()) {
		auto &resource = unreferenced_resources.top();
		unreferenced_resources.pop();

		auto &creator = resource.get().m_creator.value().get();
		if(creator.m_ref_count > 0u)
			--(creator.m_ref_count);

		if(creator.m_ref_count == 0u) {
			for(const auto &read_resource_id : creator.m_read_attachments) {
				auto &read_resource = get_resource_func(read_resource_id);

				if(read_resource.m_ref_count > 0u)
					--(read_resource.m_ref_count);

				if(read_resource.m_ref_count == 0u && read_resource.transient())
					unreferenced_resources.emplace(read_resource);
			}
		}

		for(auto &writer_id : resource.get().m_writers) {
			auto &writer = get_render_task_func(writer_id);

			if(writer.m_ref_count > 0u)
				--(creator.m_ref_count);

			if(writer.m_ref_count == 0u) {
				for(const auto &read_resource_id : writer.m_read_attachments) {
					auto &read_resource = get_resource_func(read_resource_id);

					if(read_resource.m_ref_count > 0u)
						--(read_resource.m_ref_count);

					if(read_resource.m_ref_count == 0u && read_resource.transient())
						unreferenced_resources.emplace(read_resource);
				}
			}
		}
	}

	m_timeline.clear();

	auto realized_resources = std::vector<ResourceBase::ID>{};
	auto derealized_resources = std::vector<ResourceBase::ID>{};

	const auto update_create_resources = [&realized_resources, &derealized_resources, &get_resource_func](const auto &resources){
		for(auto resource_id : resources) {
			auto &resource = get_resource_func(resource_id);

			realized_resources.emplace_back(resource_id);
			if(resource.m_readers.empty() && resource.m_writers.empty())
				derealized_resources.emplace_back(resource_id);
		}
	};

	const auto update_rw_resources = [&realized_resources, &derealized_resources](const auto &read_resources, const auto &write_resources, auto &reads_writes){
		reads_writes.reserve(std::size(read_resources) + std::size(write_resources));

		std::copy(std::cbegin(read_resources), std::cend(read_resources), std::back_inserter(reads_writes));
		std::copy(std::cbegin(write_resources), std::cend(write_resources), std::back_inserter(reads_writes));
	};


	for(auto &render_task_ptr : m_render_tasks) {
		auto &render_task = *render_task_ptr;

		if(render_task.m_ref_count == 0)
			continue;

		realized_resources = std::vector<ResourceBase::ID>{};
		derealized_resources = std::vector<ResourceBase::ID>{};

		update_create_resources(render_task.m_create_attachments);
		update_create_resources(render_task.m_create_buffers);

		auto read_write_attachments = std::vector<ResourceBase::ID>{};
		auto read_write_buffers = std::vector<ResourceBase::ID>{};

		update_rw_resources(render_task.m_read_attachments, render_task.m_write_attachments, read_write_attachments);
		update_rw_resources(render_task.m_read_buffers, render_task.m_write_buffers, read_write_buffers);

		const auto cull = [this, &render_task, &derealized_resources, &get_resource_func](auto &reads_writes) {
			for(auto resource_id : reads_writes) {
				auto &resource = get_resource_func(resource_id);

				if(!resource.transient())
					continue;

				auto valid = false;
				auto last_index = std::size_t{};

				if(!resource.m_readers.empty()) {
					const auto it = std::find_if(
								std::cbegin(m_render_tasks),
								std::cend(m_render_tasks),
								[&resource](const auto &item) -> bool {
									return item->id() == resource.m_readers.back();
								}
					);

					if(it != std::cend(m_render_tasks)) {
						valid = true;
						last_index = std::distance(std::cbegin(m_render_tasks), it);
					}
				}

				if(!resource.m_writers.empty()) {
					const auto it = std::find_if(
								std::cbegin(m_render_tasks),
								std::cend(m_render_tasks),
								[&resource](const auto &item) -> bool {
									return item->id() == resource.m_writers.back();
								}
					);

					if(it != std::cend(m_render_tasks)) {
						valid = true;
						last_index = std::max(last_index, static_cast<std::size_t>(std::distance(std::cbegin(m_render_tasks), it)));
					}
				}

				if(valid && m_render_tasks[last_index]->id() == render_task.id())
					derealized_resources.emplace_back(resource_id);
			}
		};

		cull(read_write_attachments);
		cull(read_write_buffers);

		render_task.initRenderPass(m_resources);
		m_timeline.emplace_back(render_task.id(), std::move(realized_resources), std::move(derealized_resources));
	}
}

/////////////////////////////////////
/////////////////////////////////////
void RenderGraph::execute(CommandBuffer &command_buffer) {
	for(auto &step : m_timeline) {
		for(auto resource_id : step.realized_resources) {
			auto &resource = m_resources.getResource(resource_id);
			resource.realize(m_device.get());
		}

		auto &task = getRenderTask(step.task);

		task.execute(command_buffer);

		for(auto resource_id : step.derealized_resources) {
			auto &resource = m_resources.getResource(resource_id);
			resource.derealize();
		}
	}
}

/////////////////////////////////////
/////////////////////////////////////
void RenderGraph::clear() {
	m_render_tasks.clear();
	m_resources.clear();
}


/////////////////////////////////////
/////////////////////////////////////
void RenderGraph::exportGraphviz(const _std::filesystem::path &filepath) const {
	const auto get_render_task_func = [this](auto id) -> const RenderTaskBase & {return getRenderTask(id);};
	const auto get_resource_func = [this](auto id) -> const ResourceBase& {return m_resources.getResource(id);};

	const auto print_resource_func = [&get_resource_func](auto &stream, auto &resources) {
		for(const auto &resource_id : resources) {
			const auto &resource = get_resource_func(resource_id);
			stream << "\"" << resource.name() << "\"";
		}
	};

	const auto print_whole_resource_func = [](auto &stream, const auto &resources) {
		for(const auto &resource : resources)
			stream << "\"" << resource->name() << "\" [label=\""
				   << resource->name() << "\\nRefs: " << resource->m_ref_count
				   << "\\nID: " << resource->m_id << "\", style=filled, fillcolor= "
				   << (resource->transient() ? "skyblue" : "steelblue") << "]\n";
	};

	const auto print_readers_func = [&get_render_task_func](auto &stream, const auto &resources) {
		for(const auto &resource : resources) {
			stream << "\"" << resource->name() << "\" -> {";
			for(const auto &render_task_id : resource->m_readers) {
				auto &render_task = get_render_task_func(render_task_id);
				stream << "\"" << render_task.name() << "\" ";
			}
			stream << "} [color=firebrick]\n";
		}
	};

	auto stream = std::ofstream{filepath.c_str()};
	stream << "digraph RenderGraph \n{\n";

	stream << "rankdir = LR\n";
	stream << "bgcolor = black\n\n";
	stream << "node [shape=rectangle, fontname=\"helvetica\", fontsize=12]\n\n";

	for(const auto &render_task : m_render_tasks)
		stream << "\"" << render_task->name() << "\" [label=\"" << render_task->name() << "\\nRefs: " << render_task->m_ref_count << "\\nID: " << render_task->m_id << "\", style=filled, fillcolor=darkorange]\n";
	stream << "\n";

	print_whole_resource_func(stream, m_resources.textures());
	print_whole_resource_func(stream, m_resources.buffers());

	stream << "\n";

	for(const auto &render_task : m_render_tasks) {
		stream << "\"" << render_task->name() << "\" -> {";

		print_resource_func(stream, render_task->m_create_attachments);
		print_resource_func(stream, render_task->m_create_buffers);

		stream << "} [color=seagreen]\n";

		stream << "\"" << render_task->name() << "\" -> {";

		print_resource_func(stream, render_task->m_write_attachments);
		print_resource_func(stream, render_task->m_write_buffers);

		stream << "} [color=gold]\n";
	}
	stream << "\n";

	print_readers_func(stream, m_resources.textures());
	print_readers_func(stream, m_resources.buffers());

	stream << "}" << std::flush;
}

/////////////////////////////////////
/////////////////////////////////////
const RenderTaskBase &RenderGraph::getRenderTask(RenderTaskBase::ID render_task_id) const noexcept {
	ASSERT(render_task_id < m_next_task_id, "Render task id out of bound");
	
	auto it = std::find_if(std::begin(m_render_tasks), std::end(m_render_tasks), [render_task_id](const auto &task) {
		return task->id() == render_task_id;
	});
	
	ASSERT(it != std::end(m_render_tasks), core::format("Failed to find render task, id %{1}", render_task_id));
	
	return *it->get();
}

/////////////////////////////////////
/////////////////////////////////////
RenderTaskBase &RenderGraph::getRenderTask(RenderTaskBase::ID render_task_id) noexcept {
	ASSERT(render_task_id < m_next_task_id, "Render task id out of bound");
	
	auto it = std::find_if(std::begin(m_render_tasks), std::end(m_render_tasks), [render_task_id](const auto &task) {
		return task->id() == render_task_id;
	});
	
	ASSERT(it != std::end(m_render_tasks), core::format("Failed to find render task, id %{1}", render_task_id));
	
	return *it->get();
}
