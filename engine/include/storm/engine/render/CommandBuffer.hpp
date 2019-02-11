// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/PipelineState.hpp>
#include <storm/engine/render/BindingState.hpp>
#include <storm/engine/render/Utils.hpp>

#include <storm/core/Memory.hpp>
#include <storm/core/NonCopyable.hpp>
#include <storm/core/Pimpl.hpp>

namespace storm::engine {
	class CommandBufferImpl;
	class CommandBuffer : public core::NonCopyable {
		public:
			Unique_Object(CommandBuffer)

			explicit CommandBuffer(const Device &device, bool primary = true);
			~CommandBuffer();

			CommandBuffer(CommandBuffer &&);
			CommandBuffer &operator=(CommandBuffer &&);

			void reset();

			void begin(bool one_time_submit = false);
			void end();

			void beginRenderPass(RenderPass &render_pass, const Framebuffer &buffer, core::RGBColorF clear_color = core::RGBColorDef::Black);
			void endRenderPass();

			void draw(std::size_t vertex_count, std::size_t instance_count = 1, 
					  std::uint32_t first_vertex = 0, std::uint32_t first_instance = 0);
			void drawIndexed(std::size_t index_count, std::size_t instance_count = 1, 
							 std::uint32_t first_index = 0, std::int32_t vertex_offset = 0, std::uint32_t first_instance = 0);
			 
			void submit(const std::vector<const Semaphore*> &wait_semaphores, 
						const std::vector<const Semaphore*> &signal_semaphores, std::vector<PipelineStage> pipeline_states = { PipelineStage::COLOR_ATTACHMENT_OUTPUT }, const Fence *fence = nullptr);

			void setProgram(const Program &program);

			void bindVertexBuffer(std::uint32_t index, const VertexBuffer &buffer);
			void bindIndexBuffer(const IndexBuffer &buffer, bool large_indices = false);
			
			void executeCommandBuffers(const std::vector<std::reference_wrapper<CommandBuffer>> &command_buffers);
			
			PipelineState &pipelineState() noexcept;
			const PipelineState &pipelineState() const noexcept;

			BindingState &bindingState() noexcept;
			const BindingState &bindingState() const noexcept;

			IMPLEMENTATION(CommandBufferImpl)
		private:
			core::Pimpl<CommandBufferImpl> m_impl;
	};
}
