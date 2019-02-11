// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/render/Surface.hpp>

#include <storm/engine/scenegraph/SceneRenderer.hpp>
#include <storm/engine/scenegraph/Scene.hpp>

#include "DefaultRenderTaskData.hpp"

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
SceneRenderer::SceneRenderer(const Device &device,
							 const Surface &surface,
							 uvec2 render_extent)
	: m_device{device},
	  m_surface{surface},
	  m_render_extent{render_extent},
	  m_render_graph{m_device},
	  m_fence{m_device},
	  m_semaphore{m_device},
	  m_current_command_buffer{0u},
	  m_backbuffer_desc {
		1u,
		Format::RGBA8888UNORM,
		{m_render_extent, 1u}
	  },
	  m_depthbuffer_desc {
		  1u,
		  m_device.get().bestDepthFormat(),
		  {m_render_extent, 1u}
	  },
	  m_camera_buffer_desc {
		sizeof(CameraData),
		alignof(CameraData)
	  },
	  m_camera_buffer{m_device, m_camera_buffer_desc},
	  m_meshdata_buffer_desc {
		sizeof(MeshData),
		alignof(MeshData)
	  }
	{
	
	m_command_buffers.reserve(4u);
	
	for(auto i = 0u; i < 4u; ++i) {
		m_command_buffers.emplace_back(m_device);
	}
	
	auto camera = CameraData{};
	
	m_camera_buffer.updateData(reinterpret_cast<const std::byte *>(&camera), sizeof(camera), 0);
	
	const auto &vert_data = m_shader_library.getSource(ShaderLibrary::FORWARD_RENDER_VERT);
	const auto &frag_data = m_shader_library.getSource(ShaderLibrary::FORWARD_RENDER_FRAG);
	m_forward_vert = m_device.get().createShaderPtr(engine::Shader::Stage::VERTEX, std::data(vert_data), std::size(vert_data));
	m_forward_frag = m_device.get().createShaderPtr(engine::Shader::Stage::FRAGMENT, std::data(frag_data), std::size(frag_data));

	m_forward_program = m_device.get().createProgramPtr();

	m_forward_program->addShaderModule(*m_forward_vert);
	m_forward_program->addShaderModule(*m_forward_frag);
	m_forward_program->link();
	
	auto &begin_pass = m_render_passes["begin_task"];
	begin_pass = m_device.get().createRenderPassPtr();
	begin_pass->setExtent(m_render_extent);
	begin_pass->addAttachment(m_backbuffer_desc.format);
	begin_pass->addAttachment(m_depthbuffer_desc.format);

	auto subpass = RenderPass::SubPass{
		{RenderPass::SubPass::EXTERNAL},
		{},
		{0, 1}
	};

	begin_pass->addSubPass(subpass);
	begin_pass->build();
	
	m_backbuffer = Framebuffer::makeUnique(m_device.get(), begin_pass->implementation());
	
	auto &forward_render_pass = m_render_passes["forward_render_task"];
	forward_render_pass = m_device.get().createRenderPassPtr();
	forward_render_pass->setExtent(m_render_extent);
	forward_render_pass->addAttachment(m_backbuffer_desc.format);
	forward_render_pass->addAttachment(m_depthbuffer_desc.format);

	forward_render_pass->addSubPass(subpass);
	forward_render_pass->build();
}

/////////////////////////////////////
/////////////////////////////////////
SceneRenderer::~SceneRenderer() = default;

/////////////////////////////////////
/////////////////////////////////////
SceneRenderer::SceneRenderer(SceneRenderer &&) = default;

/////////////////////////////////////
/////////////////////////////////////
SceneRenderer &SceneRenderer::operator=(SceneRenderer &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void SceneRenderer::render(Scene &scene) {
	//m_device.get().waitIdle();

	updateRenderGraph(scene);

	m_fence.wait();
	m_fence.reset();

	auto &command_buffer = m_command_buffers[m_current_command_buffer];
	command_buffer.reset();
	m_render_graph.execute();
}

/////////////////////////////////////
/////////////////////////////////////
void SceneRenderer::exportRenderGraph(const _std::filesystem::path &filepath) const {
	m_render_graph.exportGraphviz(filepath);
}

/////////////////////////////////////
/////////////////////////////////////
void SceneRenderer::updateRenderGraph(Scene &scene) {
	m_render_graph.clear();

	const auto camera_buffer_resource = m_render_graph.addRetainedResource(
			"camera_buffer",
			m_camera_buffer_desc,
			m_camera_buffer
	);
	
	auto backbuffer_desc = Framebuffer::Description{};
	const auto backbuffer_resource = m_render_graph.addRetainedResource(
			"backbuffer",
			backbuffer_desc,
			*m_backbuffer
	);

	auto &command_buffer = m_command_buffers[m_current_command_buffer];
	auto &begin_task = m_render_graph.addRenderPass<BeginTaskData>(
		"begin_task",
		[&](BeginTaskData &data, RenderTaskBuilder &builder) {
			data.meshdata_buffer = 
				builder.create<UniformBufferResource>("meshdata_buffer", m_meshdata_buffer_desc);
			data.camera_buffer =
				builder.read<UniformBufferResource>(camera_buffer_resource);
			data.backbuffer =
				builder.read<FramebufferResource>(backbuffer_resource);
		},
		[&](const BeginTaskData &, ResourcePool &) {
			command_buffer.begin();
		}
	);
	begin_task.setCullImune(true);

	const auto &begin_task_data = begin_task.data();

	addDefaultForwardRenderTask(scene, begin_task_data);

	auto &submit_task = m_render_graph.addRenderPass<SubmitTaskData>(
		"submit_task",
		[&](SubmitTaskData &data, RenderTaskBuilder &builder) {
			data.meshdata_buffer = builder.read<UniformBufferResource>(begin_task_data.meshdata_buffer);
			data.backbuffer = builder.read<FramebufferResource>(begin_task_data.backbuffer);
		},
		[&](const SubmitTaskData &data, ResourcePool &resources) {
			command_buffer.end();

			command_buffer.submit({}, { &m_semaphore });
			
			auto &backbuffer = resources.acquireResourceAs<FramebufferResource>(data.backbuffer).resource();

			auto &surface = const_cast<Surface &>(m_surface.get());
			surface.presentFrame(backbuffer, m_semaphore, m_fence);
		}
	);
	submit_task.setCullImune(true);

	m_render_graph.compile();
	
	m_current_command_buffer = (m_current_command_buffer + 1) % std::size(m_command_buffers);
}

/////////////////////////////////////
/////////////////////////////////////
void SceneRenderer::addDefaultForwardRenderTask(Scene &scene, const BeginTaskData &begin_task_data) {
	const auto &meshes = scene.meshes().meshes();
	auto &command_buffer = m_command_buffers[m_current_command_buffer];

	auto &forward_render_task = m_render_graph.addRenderPass<ForwardRenderTaskData>(
		"forward_render_task",
		[&](ForwardRenderTaskData &data, RenderTaskBuilder &builder) {
			data.meshdata_buffer = 
				builder.write<UniformBufferResource>(begin_task_data.meshdata_buffer);
			data.camera_buffer = 
				builder.write<UniformBufferResource>(begin_task_data.camera_buffer);
			data.backbuffer = 
				builder.write<FramebufferResource>(begin_task_data.backbuffer);
		},
		[&](const ForwardRenderTaskData &data, ResourcePool &resources) {
			auto &render_pass = m_render_passes["forward_render_task"];
			
			auto &camera_buffer = resources.acquireResourceAs<UniformBufferResource>(data.camera_buffer);
			auto &meshdata_buffer = resources.acquireResourceAs<UniformBufferResource>(data.meshdata_buffer);
			const auto &backbuffer = resources.acquireResourceAs<FramebufferResource>(data.backbuffer).resource();
			
			command_buffer.pipelineState().viewport =
				Viewport { 0.f, 0.f, static_cast<float>(m_render_extent.x), static_cast<float>(m_render_extent.y) };
			command_buffer.pipelineState().scissor =
				vec4 { 0.f, 0.f, m_render_extent };
			command_buffer.pipelineState().cull_mode =
				CullMode::BACK;
			command_buffer.pipelineState().is_depth_test_enabled = true;
			command_buffer.bindingState().bindings.emplace_back(
				UniformBufferBinding {
					&camera_buffer.resource(),
					0u,
					camera_buffer.description().size,
					0u,
					Shader::Stage::VERTEX
				}
			);
			command_buffer.bindingState().bindings.emplace_back(
				UniformBufferBinding {
					&meshdata_buffer.resource(),
					1u,
					meshdata_buffer.description().size,
					0u,
					Shader::Stage::VERTEX
				}
			);

			command_buffer.setProgram(*m_forward_program);
			command_buffer.beginRenderPass(*render_pass, backbuffer);
			for(const auto &mesh : meshes) {
				if(m_camera.projection != mesh.projection) {
					m_camera.projection = mesh.projection;
					camera_buffer.resource().updateData(reinterpret_cast<const std::byte *>(&m_camera.projection), sizeof(m_camera.projection), 0u);
				} if(m_camera.view != mesh.view) {
					m_camera.view = mesh.view;
					camera_buffer.resource().updateData(reinterpret_cast<const std::byte *>(&m_camera.view), sizeof(m_camera.view), offsetof(CameraData, view));
				}
				
				auto meshdata = MeshData { mesh.model };
				meshdata_buffer.resource().updateData(reinterpret_cast<const std::byte *>(&meshdata), sizeof(meshdata), 0u);
				
				command_buffer.pipelineState().vertex_input_state = mesh.vertex_state;
				command_buffer.bindVertexBuffer(0, *mesh.vertex_buffer);

				if(mesh.index_buffer) {
					command_buffer.bindIndexBuffer(*mesh.index_buffer, mesh.large_indices);
					command_buffer.drawIndexed(mesh.index_count);
				} else
					command_buffer.draw(mesh.vertex_count);
			}

			command_buffer.endRenderPass();
		}
	);
	forward_render_task.setCullImune(true);
}
