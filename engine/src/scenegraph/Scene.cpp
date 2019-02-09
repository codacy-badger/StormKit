// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/render/Surface.hpp>

#include <storm/engine/scenegraph/Scene.hpp>
#include <storm/engine/scenegraph/CameraNode.hpp>
#include <storm/engine/scenegraph/GeometryNode.hpp>
#include <storm/engine/scenegraph/TransformNode.hpp>

#include "DefaultRenderTaskData.hpp"

#include <fstream>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
constexpr std::string_view colorFromNode(std::string_view name) {
	if(name == "CameraNode")
		return ".7 .3 1.0";
	else if(name =="ProgramNode")
		return "0.3 0.2 0.5";
	else if(name =="GroupNode")
		return "0.9 0.1 0.3";
	else if(name =="MaterialNode")
		return "0.4 0.1 0.4";
	else if(name =="GeometryNode")
		return "0.1 0.2 0.3";
	else if(name =="TransformNode")
		return "0.1 0.2 0.7";

	return "1.0 1.0 1.0";
}

/////////////////////////////////////
/////////////////////////////////////
template <typename T>
constexpr bool is(const SceneNode *node) {
	return dynamic_cast<const T*>(node) != nullptr;
}

/////////////////////////////////////
/////////////////////////////////////
Scene::Scene(const Device &device, const Surface &surface, uvec2 render_extent)
	: m_device{device},
	  m_surface{surface},
	  m_render_extent{std::move(render_extent)},
	  m_fence{m_device},
	  m_semaphore{m_device},
	  m_tree{m_device},
	  m_root{*this},
	  m_render_graph{m_device},
	  m_backbuffer_desc {
		1u,
		Format::RGBA8888UNORM,
		{m_render_extent, 1u}
	  },
	  m_camera_buffer_desc {
		sizeof(CameraMatrices),
		alignof (CameraMatrices)
	  }
	{
	m_root.notify(NodeEvent::ADDED);
	m_root.setObserver(&m_tree);

	m_camera_buffer = m_device.get().createUniformBufferPtr(m_camera_buffer_desc);
}

/////////////////////////////////////
/////////////////////////////////////
Scene::~Scene() = default;

/////////////////////////////////////
/////////////////////////////////////
Scene::Scene(Scene &&) = default;

/////////////////////////////////////
/////////////////////////////////////
Scene &Scene::operator=(Scene &&) = default;


/////////////////////////////////////
/////////////////////////////////////
void Scene::update() {
	m_tree.traverse(m_cache);

	updateRenderGraph();
}

/////////////////////////////////////
/////////////////////////////////////
void Scene::exportSceneGraph(const _std::filesystem::path &filepath) const {
	using namespace std::literals;

	auto stream = std::ofstream{filepath, std::ios::out};

	stream << "digraph G { \n"
		   << "    rankdir = LR\n"
		   << "    bgcolor = black\n\n"
		   << "    node [shape=rectangle, fontname=\"helvetica\", fontsize=12];\n\n";

	auto node_stack = std::stack<const SceneNode*>{};
	auto node_done = std::vector<const SceneNode*>{};
	node_stack.emplace(&m_root);

	while(!node_stack.empty()) {
		auto node = node_stack.top();
		node_stack.pop();

		auto name = "Unknown Node"sv;

		if(is<CameraNode>(node)) name = nameOfNode<CameraNode>();
		else if(is<GeometryNode>(node)) name = nameOfNode<GeometryNode>();
		else if(is<TransformNode>(node)) name = nameOfNode<TransformNode>();
		else if(is<GroupNode>(node)) name = nameOfNode<GroupNode>();

		auto color = colorFromNode(name);
		const auto id = node->id();

		stream << "    \"node" << id << "\" [label=\"id: " << id << " type: " << name
			   << "\", style=filled,color=\"" << color << "\"];\n";

		stream << "    \"node" << id << "\" -> {\n";
		for(const auto &child : node->children()) {
			stream << "        \"node" << child.get().id() << "\" ;\n";

			if(std::find(std::begin(node_done), std::end(node_done), &(child.get())) == std::end(node_done))
				node_stack.emplace(&(child.get()));
		}
		stream << "    } [color=seagreen]\n\n";

		node_done.emplace_back(node);
	}

	stream << "}" << std::flush;

	stream.close();
}

/////////////////////////////////////
/////////////////////////////////////
void Scene::exportSceneTree(const _std::filesystem::path &filepath) const {
	m_tree.exportSceneTree(filepath, colorFromNode);
}

/////////////////////////////////////
/////////////////////////////////////
void Scene::exportRenderGraph(const _std::filesystem::path &filepath) const {
	m_render_graph.exportGraphviz(filepath);
}

/////////////////////////////////////
/////////////////////////////////////
void Scene::updateRenderGraph() {
	m_render_graph.clear();

	const auto camera_buffer_resource = m_render_graph.addRetainedResource(
			"camera_buffer",
			m_camera_buffer_desc,
			*m_camera_buffer
	);

	const auto &begin_task = m_render_graph.addRenderPass<BeginTaskData>(
		"begin_task",
		[&](BeginTaskData &data, RenderTaskBuilder &builder) {
			data.command_buffer = m_device.get().createCommandBufferPtr();

			data.camera_buffer = builder.read<UniformBufferResource>(camera_buffer_resource);
			data.backbuffer = builder.create<TextureResource>("backbuffer", m_backbuffer_desc);
		},
		[=](const BeginTaskData &data) {
			data.command_buffer->begin();
		}
	);
	const auto &begin_task_data = begin_task.data();

	addDefaultForwardRenderTask(begin_task_data);

	m_render_graph.addRenderPass<SubmitTaskData>(
		"submit_task",
		[&](SubmitTaskData &data, RenderTaskBuilder &builder) {
			data.command_buffer = begin_task_data.command_buffer.get();

			data.backbuffer = builder.read<UniformBufferResource>(camera_buffer_resource);
			data.backbuffer = builder.read<TextureResource>(begin_task_data.backbuffer);
		},
		[this](const SubmitTaskData &data) {
			data.command_buffer->end();

			m_fence.wait();
			m_fence.reset();

			data.command_buffer->submit({}, { &m_semaphore });
		}
	);

	m_render_graph.compile();
}

/////////////////////////////////////
/////////////////////////////////////
void Scene::addDefaultForwardRenderTask(const BeginTaskData &begin_task_data) {
	m_render_graph.addRenderPass<ForwardRenderTaskData>(
		"forward_render_task",
		[&](ForwardRenderTaskData &data, RenderTaskBuilder &builder) {
			data.command_buffer = begin_task_data.command_buffer.get();

			data.camera_buffer = builder.read<UniformBufferResource>(begin_task_data.camera_buffer);
			data.backbuffer = builder.read<TextureResource>(begin_task_data.backbuffer);

			data.render_pass = m_device.get().createRenderPassPtr();
			data.render_pass->setExtent(m_render_extent);
			data.render_pass->addAttachment(m_backbuffer_desc.format);

			auto subpass = RenderPass::SubPass{
				{RenderPass::SubPass::EXTERNAL},
				{},
				{0}
			};

			data.render_pass->addSubPass(subpass);

			data.framebuffer = Framebuffer::makeUnique(m_device.get(), data.render_pass->implementation());
		},
		[this](const ForwardRenderTaskData &data) {
			auto &camera_buffer = m_render_graph.pool().acquireResourceAs<UniformBufferResource>(data.camera_buffer);

			data.command_buffer->bindingState().bindings.emplace_back(
				UniformBufferBinding {
					&camera_buffer.resource(),
					0u,
					camera_buffer.description().size,
					0u,
					Shader::Stage::VERTEX
				}
			);

			data.command_buffer->beginRenderPass(*data.render_pass, *data.framebuffer);
			for(const auto &mesh : m_cache.meshes()) {
				data.command_buffer->bindVertexBuffer(0, *mesh.vertex_buffer);

				if(mesh.index_buffer) {
					data.command_buffer->bindIndexBuffer(*mesh.index_buffer, mesh.large_indices);
					data.command_buffer->drawIndexed(mesh.index_count);
				} else
					data.command_buffer->draw(mesh.vertex_count);
			}

			data.command_buffer->endRenderPass();
		}
	);
}
