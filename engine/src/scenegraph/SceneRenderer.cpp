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
	  m_command_buffer{m_device},
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
	  }
	{
	m_camera_buffer = m_device.get().createUniformBufferPtr(m_camera_buffer_desc);

	const auto &vert_data = m_shader_library.getSource(ShaderLibrary::FORWARD_RENDER_VERT);
	const auto &frag_data = m_shader_library.getSource(ShaderLibrary::FORWARD_RENDER_FRAG);
	m_forward_vert = m_device.get().createShaderPtr(engine::Shader::Stage::VERTEX, std::data(vert_data), std::size(vert_data));
	m_forward_frag = m_device.get().createShaderPtr(engine::Shader::Stage::FRAGMENT, std::data(frag_data), std::size(frag_data));

	m_forward_program = m_device.get().createProgramPtr();

	m_forward_program->addShaderModule(*m_forward_vert);
	m_forward_program->addShaderModule(*m_forward_frag);
	m_forward_program->link();
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
	m_device.get().waitIdle();

	updateRenderGraph(scene);

	m_fence.wait();
	m_fence.reset();

	m_command_buffer.reset();
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
			*m_camera_buffer
	);

	auto &begin_task = m_render_graph.addRenderPass<BeginTaskData>(
		"begin_task",
		[&](BeginTaskData &data, RenderTaskBuilder &builder) {
			data.render_pass = m_device.get().createRenderPassPtr();
			data.render_pass->setExtent(m_render_extent);
			data.render_pass->addAttachment(m_backbuffer_desc.format);
			data.render_pass->addAttachment(m_depthbuffer_desc.format);

			auto subpass = RenderPass::SubPass{
				{RenderPass::SubPass::EXTERNAL},
				{},
				{0, 1}
			};

			data.render_pass->addSubPass(subpass);
			data.render_pass->build();

			data.framebuffer = Framebuffer::makeUnique(m_device.get(), data.render_pass->implementation());

			data.camera_buffer = builder.read<UniformBufferResource>(camera_buffer_resource);
			data.backbuffer = builder.create<TextureResource>("backbuffer", m_backbuffer_desc);
		},
		[&](const BeginTaskData &data) {
			m_command_buffer.begin();
		}
	);
	begin_task.setCullImune(true);

	const auto &begin_task_data = begin_task.data();

	addDefaultForwardRenderTask(scene, begin_task_data);

	auto &submit_task = m_render_graph.addRenderPass<SubmitTaskData>(
		"submit_task",
		[&](SubmitTaskData &data, RenderTaskBuilder &builder) {
			data.framebuffer = begin_task_data.framebuffer.get();

			data.backbuffer = builder.read<UniformBufferResource>(camera_buffer_resource);
			data.backbuffer = builder.read<TextureResource>(begin_task_data.backbuffer);
		},
		[&](const SubmitTaskData &data) {
			m_command_buffer.end();

			m_command_buffer.submit({}, { &m_semaphore });

			auto &surface = const_cast<Surface &>(m_surface.get());
			surface.presentFrame(*data.framebuffer, m_semaphore, m_fence);
		}
	);
	submit_task.setCullImune(true);

	m_render_graph.compile();
}

/////////////////////////////////////
/////////////////////////////////////
void SceneRenderer::addDefaultForwardRenderTask(Scene &scene, const BeginTaskData &begin_task_data) {
	const auto &meshes = scene.meshes().meshes();

	auto &forward_render_task = m_render_graph.addRenderPass<ForwardRenderTaskData>(
		"forward_render_task",
		[&](ForwardRenderTaskData &data, RenderTaskBuilder &builder) {
			data.framebuffer = begin_task_data.framebuffer.get();

			data.camera_buffer = builder.read<UniformBufferResource>(begin_task_data.camera_buffer);
			data.backbuffer = builder.read<TextureResource>(begin_task_data.backbuffer);

			data.render_pass = m_device.get().createRenderPassPtr();
			data.render_pass->setExtent(m_render_extent);
			data.render_pass->addAttachment(m_backbuffer_desc.format);
			data.render_pass->addAttachment(m_depthbuffer_desc.format);

			auto subpass = RenderPass::SubPass{
				{RenderPass::SubPass::EXTERNAL},
				{},
				{0, 1}
			};

			data.render_pass->addSubPass(subpass);
			data.render_pass->build();
		},
		[&](const ForwardRenderTaskData &data) {
			auto &camera_buffer = m_render_graph.pool().acquireResourceAs<UniformBufferResource>(data.camera_buffer);

			m_command_buffer.pipelineState().viewport =
				Viewport { 0.f, 0.f, static_cast<float>(m_render_extent.x), static_cast<float>(m_render_extent.y) };
			m_command_buffer.pipelineState().scissor =
				vec4 { 0.f, 0.f, m_render_extent };
			m_command_buffer.pipelineState().cull_mode =
				CullMode::NONE;

			/*m_command_buffer.bindingState().bindings.emplace_back(
				UniformBufferBinding {
					&camera_buffer.resource(),
					0u,
					camera_buffer.description().size,
					0u,
					Shader::Stage::VERTEX
				}
			);*/

			m_command_buffer.setProgram(*m_forward_program);
			m_command_buffer.beginRenderPass(*data.render_pass, *data.framebuffer);
			for(const auto &mesh : meshes) {
				m_command_buffer.pipelineState().vertex_input_state = mesh.vertex_state;
				m_command_buffer.bindVertexBuffer(0, *mesh.vertex_buffer);

				if(mesh.index_buffer) {
					m_command_buffer.bindIndexBuffer(*mesh.index_buffer, mesh.large_indices);
					m_command_buffer.drawIndexed(mesh.index_count);
				} else
					m_command_buffer.draw(mesh.vertex_count);
			}

			m_command_buffer.endRenderPass();
		}
	);
	forward_render_task.setCullImune(true);
}
