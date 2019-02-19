// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <fstream>
#include <stack>
#include <storm/core/Strings.hpp>

#include <storm/engine/render/Surface.hpp>
#include <storm/engine/graphics/RenderGraph.hpp>

using namespace storm::engine;

struct SubmitTaskData {
    ResourceBase::ID backbuffer;
    const Surface *surface;
};

/////////////////////////////////////
/////////////////////////////////////
RenderGraph::RenderGraph(const Device &device, uvec2 extent, const Surface &surface)
    : m_device {device},
      m_extent{extent},
      m_next_task_id{1u},
      m_command_buffer{m_device} {
    auto backbuffer = m_resources.addRetainedResource("backbuffer", m_backbuffer);

    m_submit_task = std::make_unique<RenderTask<SubmitTaskData>>(
      "present_task",
      [&](SubmitTaskData &data, RenderTaskBuilder &builder) {
          data.backbuffer = builder.read<FramebufferAttachmentResource>(backbuffer);
          data.surface = &surface;
      },
      [&](CommandBuffer &cmd, const SubmitTaskData &data, ResourcePool &resource) {
          auto &[hash, render_pass, framebuffer] = m_render_passes.at("present_task");
          auto frame = const_cast<Surface*>(data.surface)->nextFrame();

          cmd.submit(
            {},
            {&frame.render_finished}
          );

          const_cast<Surface*>(data.surface)->present(framebuffer, frame);
      }
    );

    auto render_pass = RenderPass{m_device.get()};
    auto framebuffer = Framebuffer{m_device.get()};

    buildRenderPass(render_pass, framebuffer, *m_submit_task);

    m_render_passes.emplace(m_submit_task->name(), RenderPassAndHash{0u, std::move(render_pass), std::move(framebuffer)});
}


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
	// Compute refs
	for (auto &render_task : m_render_tasks)
		render_task->m_ref_count = std::size(render_task->m_create_resources)
		                         + std::size(render_task->m_write_resources);
	

	for (auto &resource : m_resources.resourceArray())
		resource->m_ref_count = std::size(resource->m_readers);

	auto unreferenced_resources = std::stack<ResourceBase *> {};
	for (const auto &resource : m_resources.resourceArray())
		if (resource->m_ref_count == 0 && resource->transient())
			unreferenced_resources.emplace(resource.get());

	while (!unreferenced_resources.empty()) {
		auto resource = unreferenced_resources.top();
		unreferenced_resources.pop();

		auto &creator = resource->m_creator.value().get();
		if (creator.m_ref_count > 0u)
			--(creator.m_ref_count);

		if (creator.m_ref_count == 0u && !creator.cullImune()) {
			for (const auto &read_resource_id : creator.m_read_resources) {
				auto &read_resource
				    = m_resources.acquireResource(read_resource_id);

				if (read_resource.m_ref_count > 0u)
					--(read_resource.m_ref_count);

				if (read_resource.m_ref_count == 0u
				    && read_resource.transient())
					unreferenced_resources.emplace(&read_resource);
			}
		}

		for (auto &writer_id : resource->m_writers) {
			auto &writer = getRenderTask(writer_id);

			if (writer.m_ref_count > 0u)
				--(creator.m_ref_count);

			if (writer.m_ref_count == 0u && !writer.cullImune()) {
				for (const auto &read_resource_id : writer.m_read_resources) {
					auto &read_resource
					    = m_resources.acquireResource(read_resource_id);

					if (read_resource.m_ref_count > 0u)
						--(read_resource.m_ref_count);

					if (read_resource.m_ref_count == 0u
					    && read_resource.transient())
						unreferenced_resources.emplace(&read_resource);
				}
			}
		}
	}

	// cull
	m_timeline.clear();

	auto realized_resources   = std::vector<ResourceBase::ID> {};
	auto derealized_resources = std::vector<ResourceBase::ID> {};

	for (auto &render_task_ptr : m_render_tasks) {
		auto &render_task = *render_task_ptr;

		if (render_task.m_ref_count == 0 && !render_task.cullImune())
			continue;

		realized_resources   = std::vector<ResourceBase::ID> {};
		derealized_resources = std::vector<ResourceBase::ID> {};

		for (auto &resource : m_resources.resourceArray()) {
			realized_resources.emplace_back(resource->id());
			if (resource->m_readers.empty() && resource->m_writers.empty())
				derealized_resources.emplace_back(resource->id());
		}

		auto reads_writes = std::vector<ResourceBase::ID> {};

		reads_writes.reserve(std::size(render_task.m_read_resources)
		                     + std::size(render_task.m_write_resources));

		std::copy(std::cbegin(render_task.m_read_resources),
		    std::cend(render_task.m_read_resources),
		    std::back_inserter(reads_writes));
		std::copy(std::cbegin(render_task.m_write_resources),
		    std::cend(render_task.m_write_resources),
		    std::back_inserter(reads_writes));

		for (auto resource_id : reads_writes) {
			auto &resource = m_resources.acquireResource(resource_id);

			if (!resource.transient())
				continue;

			auto valid      = false;
			auto last_index = std::size_t {};

			if (!resource.m_readers.empty()) {
				const auto it = std::find_if(std::cbegin(m_render_tasks),
				    std::cend(m_render_tasks),
				    [&resource](const auto &item) -> bool {
					    return item->id() == resource.m_readers.back();
				    });

				if (it != std::cend(m_render_tasks)) {
					valid      = true;
					last_index = std::distance(std::cbegin(m_render_tasks), it);
				}
			}

			if (!resource.m_writers.empty()) {
				const auto it = std::find_if(std::cbegin(m_render_tasks),
				    std::cend(m_render_tasks),
				    [&resource](const auto &item) -> bool {
					    return item->id() == resource.m_writers.back();
				    });

				if (it != std::cend(m_render_tasks)) {
					valid      = true;
					last_index = std::max(last_index,
					    static_cast<std::size_t>(
					        std::distance(std::cbegin(m_render_tasks), it)));
				}
			}

			if (valid && m_render_tasks[last_index]->id() == render_task.id())
				derealized_resources.emplace_back(resource_id);
		}

		m_timeline.emplace_back(render_task.id(), std::move(realized_resources),
		    std::move(derealized_resources));
	}

	// Create Renderpasses
	for (auto &step : m_timeline) {
		const auto &task = getRenderTask(step.task);
		auto        it   = m_render_passes.find(task.name());	

		const auto hasher = std::hash<std::string>{};
		auto hash = hasher(task.name());
		
		const auto hash_resources = [&](const auto &resources) {
			for(const auto &resource_id : resources) {
				const auto &resource = m_resources.acquireResource(resource_id);
				core::hash_combine(hash, hasher(resource.name()));
			}
		};
		
		hash_resources(task.m_create_resources);
		hash_resources(task.m_read_resources);
		hash_resources(task.m_write_resources);
		
		bool build_render_pass = true;
		
		if(it != std::end(m_render_passes)) {
			auto &[name, pass] = *it;
			
			if(pass.hash != hash)
				m_render_passes.erase(it);
			else
				build_render_pass = false;
		}
		
		if(build_render_pass) continue;
	
		auto render_pass = RenderPass{m_device.get()};
		auto framebuffer = Framebuffer{m_device.get()};

		buildRenderPass(render_pass, framebuffer, task);
		
		m_render_passes.emplace(task.name(), RenderPassAndHash{hash, std::move(render_pass), std::move(framebuffer)});
	}
}

/////////////////////////////////////
/////////////////////////////////////
void RenderGraph::render() {
    m_command_buffer.reset();
    m_command_buffer.begin();

	for (auto &step : m_timeline) {
        auto &task = getRenderTask(step.task);
        auto &[hash, render_pass, framebuffer] = m_render_passes.at(task.name());

        m_command_buffer.beginRenderPass(render_pass, framebuffer);
        task.execute(m_command_buffer, m_resources);
        m_command_buffer.endRenderPass();
    }

    m_command_buffer.end();
    m_submit_task->execute(m_command_buffer, m_resources);
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
	auto stream = std::ofstream {filepath.c_str()};
	stream << "digraph RenderGraph \n{\n";

	stream << "rankdir = LR\n";
	stream << "bgcolor = black\n\n";
	stream << "node [shape=rectangle, fontname=\"helvetica\", fontsize=12]\n\n";

	for (const auto &render_task : m_render_tasks)
		stream << "\"" << render_task->name() << "\" [label=\""
		       << render_task->name() << "\\nRefs: " << render_task->m_ref_count
		       << "\\nID: " << render_task->m_id
		       << "\", style=filled, fillcolor=darkorange]\n";
	stream << "\n";

	for (const auto &resource : m_resources.resourceArray())
		stream << "\"" << resource->name() << "\" [label=\"" << resource->name()
		       << "\\nRefs: " << resource->m_ref_count
		       << "\\nID: " << resource->m_id << "\", style=filled, fillcolor= "
		       << (resource->transient() ? "skyblue" : "steelblue") << "]\n";
	stream << "\n";

	for (const auto &render_task : m_render_tasks) {
		stream << "\"" << render_task->name() << "\" -> {";
		for (const auto &resource_id : render_task->m_create_resources) {
			const auto &resource = m_resources.acquireResource(resource_id);
			stream << "\"" << resource.name() << "\"";
		}
		stream << "} [color=seagreen]\n";

		stream << "\"" << render_task->name() << "\" -> {";
		for (const auto &resource_id : render_task->m_write_resources) {
			const auto &resource = m_resources.acquireResource(resource_id);
			stream << "\"" << resource.name() << "\"";
		}
		stream << "} [color=gold]\n";
	}
	stream << "\n";

	for (const auto &resource : m_resources.resourceArray()) {
		stream << "\"" << resource->name() << "\" -> {";
		for (const auto &render_task_id : resource->m_readers) {
			auto &render_task = getRenderTask(render_task_id);
			stream << "\"" << render_task.name() << "\" ";
		}
		stream << "} [color=firebrick]\n";
	}

	stream << "}" << std::flush;
}

/////////////////////////////////////
/////////////////////////////////////
const RenderTaskBase &RenderGraph::getRenderTask(
    RenderTaskBase::ID render_task_id) const noexcept {
	ASSERT(render_task_id < m_next_task_id, "Render task id out of bound");

	auto it = std::find_if(std::begin(m_render_tasks), std::end(m_render_tasks),
	    [render_task_id](
	        const auto &task) { return task->id() == render_task_id; });

	ASSERT(it != std::end(m_render_tasks),
	    core::format("Failed to find render task, id %{1}", render_task_id));

	return *it->get();
}

/////////////////////////////////////
/////////////////////////////////////
RenderTaskBase &RenderGraph::getRenderTask(
    RenderTaskBase::ID render_task_id) noexcept {
	ASSERT(render_task_id < m_next_task_id, "Render task id out of bound");

	auto it = std::find_if(std::begin(m_render_tasks), std::end(m_render_tasks),
      [render_task_id](const auto &task) {
          return task->id() == render_task_id;
      }
    );

	ASSERT(it != std::end(m_render_tasks),
	    core::format("Failed to find render task, id %{1}", render_task_id));

	return *it->get();
}

/////////////////////////////////////
/////////////////////////////////////
void RenderGraph::buildRenderPass(RenderPass &render_pass, Framebuffer &framebuffer, const RenderTaskBase &task) {
    framebuffer.setExtent({m_extent, 1u});

    auto subpass = RenderPass::SubPass {
		{RenderPass::SubPass::EXTERNAL}
	};

	for(const auto &resource : task.m_create_resources) {
        const auto *resource_ptr = m_resources.acquireResourcePtrAs<FramebufferAttachmentResource>(resource);

		if(!resource_ptr) continue;

        const auto id = framebuffer.addOutputAttachment(resource_ptr->resource());

        subpass.output_attachments.emplace_back(id);
	}
	
	render_pass.addSubPass(std::move(subpass));
	render_pass.build();
}
