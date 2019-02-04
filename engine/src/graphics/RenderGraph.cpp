// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/graphics/RenderGraph.hpp>

#include <fstream>
#include <stack>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
RenderGraph::RenderGraph(const Device &device)
	: m_device{device} {

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
void RenderGraph::removeRenderPass(const std::string name) {
	auto it = std::find_if(std::begin(m_render_tasks), std::end(m_render_tasks),
		[&name](const auto &task) -> bool {
			return task->name() == name;
		}
	);

	if(it == std::end(m_render_tasks))
		return;

	for(auto &resource : m_resources) {
		if(resource == nullptr) {
			auto it = std::find(std::begin(m_resources), std::end(m_resources), resource);
			m_resources.erase(it);

			continue;
		}

		if(resource->m_creator == it->get()) {
			auto it = std::find(std::begin(m_resources), std::end(m_resources), resource);

			for(auto &task : m_render_tasks) {
				auto reader = std::find(std::begin(task->m_read_resources), std::end(task->m_read_resources), resource.get());
				if(reader != std::end(task->m_read_resources))
					task->m_read_resources.erase(reader);

				auto writer = std::find(std::begin(task->m_write_resources), std::end(task->m_write_resources), resource.get());
				if(writer != std::end(task->m_write_resources))
					task->m_write_resources.erase(writer);

				m_resources.erase(it);
			}
		}

		auto reader = std::find(std::begin(resource->m_readers), std::end(resource->m_readers), it->get());
		if(reader != std::end(resource->m_readers))
			resource->m_readers.erase(reader);

		auto writer = std::find(std::begin(resource->m_writers), std::end(resource->m_writers), it->get());
		if(writer != std::end(resource->m_writers))
			resource->m_writers.erase(writer);
	}

	m_render_tasks.erase(it);
}

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

	for(auto &render_task : m_render_tasks)
		render_task->m_ref_count = std::size(render_task->m_create_resources) + std::size(render_task->m_write_resources);

	for(auto &resource : m_resources)
		resource->m_ref_count = std::size(resource->m_readers);

	auto unreferenced_resources = std::stack<ResourceBase *>{};
	for(const auto &resource : m_resources)
		if(resource->m_ref_count == 0 && resource->transient())
			unreferenced_resources.emplace(resource.get());

	while(!unreferenced_resources.empty()) {
		auto resource = unreferenced_resources.top();
		unreferenced_resources.pop();

		auto creator = resource->m_creator;

		if(creator->m_ref_count > 0u)
			--(creator->m_ref_count);

		if(creator->m_ref_count == 0u) {
			for(const auto &const_read_resource : creator->m_read_resources) {
				auto read_resource = const_cast<ResourceBase *>(const_read_resource);

				if(read_resource->m_ref_count > 0u)
					--(read_resource->m_ref_count);

				if(read_resource->m_ref_count == 0u && read_resource->transient())
					unreferenced_resources.emplace(read_resource);
			}
		}

		for(auto &writer : resource->m_writers) {
			if(writer->m_ref_count > 0u)
				--(creator->m_ref_count);

			if(writer->m_ref_count == 0u) {
				for(const auto &const_read_resource : writer->m_read_resources) {
					auto read_resource = const_cast<ResourceBase *>(const_read_resource);

					if(read_resource->m_ref_count > 0u)
						--(read_resource->m_ref_count);

					if(read_resource->m_ref_count == 0u && read_resource->transient())
						unreferenced_resources.emplace(read_resource);
				}
			}
		}
	}
}

/////////////////////////////////////
/////////////////////////////////////
void RenderGraph::execute() {

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
	auto stream = std::ofstream{filepath.c_str()};
	stream << "digraph RenderGraph \n{\n";

	stream << "rankdir = LR\n";
	stream << "bgcolor = black\n\n";
	stream << "node [shape=rectangle, fontname=\"helvetica\", fontsize=12]\n\n";

	for(const auto &render_task : m_render_tasks)
		stream << "\"" << render_task->name() << "\" [label=\"" << render_task->name() << "\\nRefs: " << render_task->m_ref_count << "\", style=filled, fillcolor=darkorange]\n";
	stream << "\n";

	for(const auto &resource : m_resources)
		stream << "\"" << resource->name() << "\" [label=\"" << resource->name() << "\\nRefs: " << resource->m_ref_count << "\", style=filled, fillcolor= " << (resource->transient() ? "skyblue" : "steelblue") << "]\n";
	stream << "\n";

	for(const auto &render_task : m_render_tasks) {
		stream << "\"" << render_task->name() << "\" -> {";
		for(const auto &resource : render_task->m_create_resources)
			stream << "\"" << resource->name() << "\"";
		stream << "} [color=seagreen]\n";

		stream << "\"" << render_task->name() << "\" -> {";
		for(const auto &resource : render_task->m_write_resources)
			stream << "\"" << resource->name() << "\"";
		stream << "} [color=gold]\n";
	}
	stream << "\n";

	for(const auto &resource : m_resources) {
		stream << "\"" << resource->name() << "\" -> {";
		for(const auto &render_task : resource->m_readers)
			stream << "\"" << render_task->name() << "\" ";
		stream << "} [color=firebrick]\n";
	}

	stream << "}" << std::flush;
}
