// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include "DefaultRenderTaskData.hpp"

#include <storm/engine/render/Surface.hpp>
#include <storm/engine/scenegraph/Scene.hpp>
#include <storm/engine/scenegraph/SceneRenderer.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
SceneRenderer::SceneRenderer(
    const Device &device, const Surface &surface, uvec2 render_extent)
    : m_device {device}, m_surface {surface}, m_render_extent {render_extent},
      m_render_graph {m_device}, m_fence {m_device}, m_semaphore {m_device},
      m_backbuffer_desc {1u, Format::RGBA8888UNORM, {m_render_extent, 1u}},
      m_depthbuffer_desc {
          1u, m_device.get().bestDepthFormat(), {m_render_extent, 1u}},
      m_camera_buffer_desc {sizeof(CameraData), alignof(CameraData)},
      m_camera_buffer {m_device, m_camera_buffer_desc} {

	auto camera = CameraData {};

	m_camera_buffer.updateData(
	    reinterpret_cast<const std::byte *>(&camera), sizeof(camera), 0);

	const auto &vert_data
	    = m_shader_library.getSource(ShaderLibrary::FORWARD_RENDER_VERT);
	const auto &frag_data
	    = m_shader_library.getSource(ShaderLibrary::FORWARD_RENDER_FRAG);
	m_forward_vert
	    = m_device.get().createShaderPtr(engine::Shader::Stage::VERTEX,
	        std::data(vert_data), std::size(vert_data));
	m_forward_frag
	    = m_device.get().createShaderPtr(engine::Shader::Stage::FRAGMENT,
	        std::data(frag_data), std::size(frag_data));

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
	updateRenderGraph(scene);

	m_render_graph.execute();
}

/////////////////////////////////////
/////////////////////////////////////
void SceneRenderer::exportRenderGraph(
    const _std::filesystem::path &filepath) const {
	m_render_graph.exportGraphviz(filepath);
}

/////////////////////////////////////
/////////////////////////////////////
void SceneRenderer::updateRenderGraph(Scene &scene) {
	m_render_graph.clear();
	
	auto renderer_resources = RendererResources{};

	renderer_resources.camera_buffer = m_render_graph.addRetainedResource(
	    "camera_buffer", m_camera_buffer_desc, m_camera_buffer);

	addDefaultForwardRenderTask(scene, renderer_resources);
/*
	auto &submit_task = m_render_graph.addRenderPass<SubmitTaskData>(
	    "submit_task",
	    [&](SubmitTaskData &data, RenderTaskBuilder &builder) {
		    data.meshdata_buffer = builder.read<UniformBufferResource>(
		        begin_task_data.meshdata_buffer);
		    data.backbuffer
		        = builder.read<FramebufferResource>(begin_task_data.backbuffer);
	    },
	    [&](const SubmitTaskData &data, ResourcePool &resources) {

		    auto &backbuffer
		        = resources
		              .acquireResourceAs<FramebufferResource>(data.backbuffer)
		              .resource();

		    auto &surface = const_cast<Surface &>(m_surface.get());
		    surface.presentFrame(backbuffer, m_semaphore, m_fence);

		    m_fence.wait();
		    m_fence.reset();
	    });
	submit_task.setCullImune(true);*/

	m_render_graph.compile();
}

/////////////////////////////////////
/////////////////////////////////////
void SceneRenderer::addDefaultForwardRenderTask(Scene &scene, const RendererResources &resources) {/*
	const auto &meshes         = scene.meshes().meshes();

	auto &forward_render_task
	    = m_render_graph.addRenderPass<ForwardRenderTaskData>(
	        "forward_render_task",
	        [&](ForwardRenderTaskData &data, RenderTaskBuilder &builder) {
		        const auto backbuffer_desc = Texture::Description {
					1u, Format::RGBA8888UNORM, {m_render_extent, 1u}
		        };
		        
				data.backbuffer = builder.create<TextureResource>("backbuffer", std::move(backbuffer_desc));
		        
		        data.meshdata_buffer = builder.create<UniformBufferResource>(
		            begin_task_data.meshdata_buffer);
		        
		        data.camera_buffer = builder.write<UniformBufferResource>(
		            resources.camera_buffer);
	        },
	        [&](const ForwardRenderTaskData &data, ResourcePool &resources) {
		        auto &render_pass = m_render_passes["forward_render_task"];

		        auto &camera_buffer
		            = resources.acquireResourceAs<UniformBufferResource>(
		                data.camera_buffer);
		        auto &meshdata_buffer
		            = resources.acquireResourceAs<UniformBufferResource>(
		                data.meshdata_buffer);
		        const auto &backbuffer
		            = resources
		                  .acquireResourceAs<FramebufferResource>(
		                      data.backbuffer)
		                  .resource();

		        command_buffer.pipelineState().viewport
		            = Viewport {0.f, 0.f, static_cast<float>(m_render_extent.x),
		                static_cast<float>(m_render_extent.y)};
		        command_buffer.pipelineState().scissor
		            = vec4 {0.f, 0.f, m_render_extent};
		        command_buffer.pipelineState().cull_mode = CullMode::BACK;
		        command_buffer.pipelineState().is_depth_test_enabled = true;
		        command_buffer.bindingState().bindings.emplace_back(
		            UniformBufferBinding {&camera_buffer.resource(), 0u,
		                camera_buffer.description().size, 0u,
		                Shader::Stage::VERTEX});

		        command_buffer.setProgram(*m_forward_program);
		        command_buffer.beginRenderPass(*render_pass, backbuffer);

		        auto counter = 0;
		        for (const auto &mesh : meshes) {
			        if (m_camera.projection != mesh.projection) {
				        m_camera.projection = mesh.projection;
				        camera_buffer.resource().updateData(
				            reinterpret_cast<const std::byte *>(
				                &m_camera.projection),
				            sizeof(m_camera.projection), 0u);
			        }
			        if (m_camera.view != mesh.view) {
				        m_camera.view = mesh.view;
				        camera_buffer.resource().updateData(
				            reinterpret_cast<const std::byte *>(&m_camera.view),
				            sizeof(m_camera.view), offsetof(CameraData, view));
			        }

			        const auto offset
			            = counter * static_cast<std::int32_t>(sizeof(MeshData));

			        command_buffer.bindingState().bindings.emplace_back(
			            UniformBufferBinding {&meshdata_buffer.resource(), 1u,
			                meshdata_buffer.description().size, offset,
			                Shader::Stage::VERTEX});

			        auto meshdata = MeshData {mesh.model};
			        meshdata_buffer.resource().updateData(
			            reinterpret_cast<const std::byte *>(&meshdata),
			            sizeof(meshdata), offset);

			        command_buffer.pipelineState().vertex_input_state
			            = mesh.vertex_state;
			        command_buffer.bindVertexBuffer(0, *mesh.vertex_buffer);

			        if (mesh.index_buffer) {
				        command_buffer.bindIndexBuffer(
				            *mesh.index_buffer, mesh.large_indices);
				        command_buffer.drawIndexed(mesh.index_count);
			        } else
				        command_buffer.draw(mesh.vertex_count);

			        ++counter;
		        }

		        command_buffer.endRenderPass();
	        });
	forward_render_task.setCullImune(true);*/
}
