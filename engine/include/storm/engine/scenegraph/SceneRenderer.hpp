// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <functional>

#include <storm/core/NonCopyable.hpp>
#include <storm/core/Filesystem.hpp>

#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/Types.hpp>
#include <storm/engine/render/Framebuffer.hpp>

#include <storm/engine/graphics/RenderGraph.hpp>
#include <storm/engine/graphics/ShaderLibrary.hpp>


#include <storm/engine/scenegraph/ForwardDeclarations.hpp>

namespace storm::engine {
	struct BeginTaskData;
	class SceneRenderer : public core::NonCopyable {
		public:
			explicit SceneRenderer(const Device &device, const Surface &surface, uvec2 render_extent);
			~SceneRenderer();

			SceneRenderer(SceneRenderer &&);
			SceneRenderer &operator=(SceneRenderer &&);

			void render(Scene &scene);

			void exportRenderGraph(const _std::filesystem::path &filepath) const;
		private:
			void updateRenderGraph(Scene &scene);
			void addDefaultForwardRenderTask(Scene &scene, const BeginTaskData &begin_task_data);

			struct MeshData {
				mat4 transform;
			};

			struct CameraData {
				mat4 projection = mat4{1.f};
				mat4 view = mat4{1.f};
			} m_camera;

			std::reference_wrapper<const Device>  m_device;
			std::reference_wrapper<const Surface> m_surface;
			uvec2 m_render_extent;

			RenderGraph m_render_graph;
			ShaderLibrary m_shader_library;

			Shader::Ptr  m_forward_vert;
			Shader::Ptr  m_forward_frag;
			Program::Ptr m_forward_program;

			Fence         m_fence;
			Semaphore     m_semaphore;
			std::vector<CommandBuffer> m_command_buffers;
			std::uint32_t m_current_command_buffer;

			Texture::Description m_backbuffer_desc;
			Texture::Description m_depthbuffer_desc;

			UniformBuffer::Description m_camera_buffer_desc;
			UniformBuffer         m_camera_buffer;
			
			UniformBuffer::Description m_meshdata_buffer_desc;
			UniformBuffer         m_meshdata_buffer;

			std::unordered_map<std::string, RenderPass::Ptr> m_render_passes;
			Framebuffer::Ptr m_backbuffer;
	};
}
