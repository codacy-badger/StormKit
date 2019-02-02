// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/UniformBuffer.hpp>
#include <storm/engine/render/Semaphore.hpp>
#include <storm/engine/render/Fence.hpp>
#include <storm/engine/render/RenderPass.hpp>
#include <storm/engine/render/PipelineState.hpp>

#include <storm/engine/graphics/Hashes.hpp>
#include <storm/engine/graphics/Drawable.hpp>
#include <storm/engine/graphics/DrawableCache.hpp>
#include <storm/engine/graphics/RendererCommandBufferCacheKey.hpp>

#include <map>
#include <unordered_map>

namespace storm::engine {
	struct Matrices {
		mat4 projection;
		mat4 view;
	};

	class Renderer : public storm::core::NonCopyable {
		public:
			SUR_Object(Renderer)
			
			explicit Renderer(Device &device, Surface &surface, uvec2 internal_extent);
			~Renderer();
			
			Renderer(Renderer &&);
			
			inline PipelineState &basePipelineState() noexcept { return m_base_pipeline_state; }
			inline BindingState &baseBindingState() noexcept { return m_base_binding_state; }
			inline Matrices &matrices() noexcept { m_matrices_is_updated = true; return m_matrices; }
			inline const Matrices &matrices() const noexcept { return m_matrices; }
			
			void prepareRender();
			void render(const DrawableCache &cache);
			void renderDrawable(const Drawable &drawable);
			void presentFrame();
		private:
			CommandBuffer &buildOrAcquireCompatibleCommandBuffer(const Drawable &drawable);
			void uploadMatrices();
			
			Device  &m_device;
			Surface &m_surface;
			
			Fence     m_render_finished_fence;
			Semaphore m_semaphore;
			
			RenderPass m_forward_render_pass;
			
			uvec2 m_internal_extent;
			
			PipelineState m_base_pipeline_state;
			BindingState  m_base_binding_state;
			
			bool          m_matrices_is_updated;
			Matrices      m_matrices;
			UniformBuffer m_matrices_uniform_buffer;
			
			std::unordered_map<Drawable, UniformBuffer> m_uniform_buffers;
			std::unordered_map<Drawable, VertexBuffer> m_vertex_buffers;
			std::unordered_map<Drawable, IndexBuffer> m_index_buffers;
			std::unordered_map<RendererCommandBufferCacheKey, std::reference_wrapper<CommandBuffer>> m_command_buffer_table;
			
			std::vector<CommandBuffer> m_command_buffers;
	};
}
