// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/Numerics.hpp>
#include <storm/engine/render/BindingState.hpp>
#include <storm/engine/render/CommandBuffer.hpp>
#include <storm/engine/render/RenderPass.hpp>
#include <storm/engine/vulkan/PipelineCache.hpp>
#include <storm/engine/vulkan/UniqueHandle.hpp>
#include <unordered_map>
#include <utility>

namespace storm::engine {
	class VertexBuffer;
	class IndexBuffer;
	class CommandBufferImpl {
	public:
		explicit CommandBufferImpl(const Device &device, bool primary = true);
		~CommandBufferImpl();

		CommandBufferImpl(CommandBufferImpl &&);

		void reset();

		void begin(bool one_time_submit) const;
		void end() const;

		void beginRenderPass(RenderPass &render_pass, const Framebuffer &buffer,
		    core::RGBColorF clear_color);
		void endRenderPass();

		void draw(std::size_t vertex_count, std::size_t instance_count,
		    std::uint32_t first_vertex, std::uint32_t first_instance);
		void drawIndexed(std::size_t index_count, std::size_t instance_count,
		    std::uint32_t first_index, std::int32_t vertex_offset,
		    std::uint32_t first_instance);

		void submit(const std::vector<const Semaphore *> &wait_semaphores,
		    const std::vector<const Semaphore *> &        signal_semaphores,
		    std::vector<PipelineStage> pipeline_states, const Fence *fence);

		void setProgram(const Program &program);

		void transitionImageLayout(vk::Image image, vk::ImageLayout old_layout,
		    vk::ImageLayout new_layout, vk::PipelineStageFlagBits source,
		    vk::PipelineStageFlagBits destination,
		    vk::ImageSubresourceRange range
		    = {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1});
		void blitImage(vk::Image source, vk::Image destination,
		    const glm::uvec2 &src_size, const glm::uvec2 &destination_size,
		    vk::Filter filter) const;
		void blitImage(vk::Image source, uint32_t source_mipmap_level,
		    vk::Image destination, uint32_t destination_mipmap_level,
		    const glm::uvec2 &src_size, const glm::uvec2 &destination_size,
		    uint32_t layer_count, vk::Filter filter) const;
		void copyBuffer(const vk::Buffer &src, const vk::Buffer &dest,
		    vk::DeviceSize size) const;
		void copyBufferToImage(const vk::Buffer &src, const vk::Image &dest,
		    std::uint32_t width, std::uint32_t height) const;

		void bindVertexBuffer(std::uint32_t index, const VertexBuffer &buffer);
		void bindIndexBuffer(const IndexBuffer &buffer, bool large_indices);

		void executeCommandBuffers(
		    const std::vector<std::reference_wrapper<CommandBuffer>>
		        &command_buffers);

		inline PipelineState &      pipelineState() noexcept;
		inline const PipelineState &pipelineState() const noexcept;

		inline BindingState &      bindingState() noexcept;
		inline const BindingState &bindingState() const noexcept;

		inline const vk::CommandBuffer &vkCommandBuffer() const noexcept;

	private:
		void                           createCommandBuffer();
		void                           createDescriptorPool();
		void                           bindDescriptors();
		const vk::Pipeline &           getOrCreatePipeline();
		std::vector<vk::DescriptorSet> getOrCreateDescriptorSets();

		UniqueHandle<vk::CommandBuffer> m_command_buffer;

		PipelineState m_pipeline_state;
		bool          m_is_pipeline_state_dirty;

		BindingState m_binding_state;
		bool         m_is_binding_state_dirty;

		bool m_is_primary_command_buffer;

		UniqueHandle<vk::DescriptorPool>             m_descriptor_pool;
		std::vector<UniqueHandle<vk::DescriptorSet>> m_descriptor_sets;

		const RenderPass *        m_current_render_pass;
		const Program *           m_current_program;
		const vk::PipelineLayout *m_current_pipeline_layout;

		const Device &m_device;
	};
}

#include "CommandBufferImpl.inl"
