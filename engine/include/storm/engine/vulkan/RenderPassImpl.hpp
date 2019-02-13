// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/render/RenderPass.hpp>
#include <storm/engine/vulkan/FramebufferImpl.hpp>
#include <storm/engine/vulkan/UniqueHandle.hpp>

namespace storm::engine {
	class RenderPassImpl : public core::NonCopyable {
	public:
		explicit RenderPassImpl(const Device &device);
		~RenderPassImpl();

		RenderPassImpl(RenderPassImpl &&);

		void init();

		std::size_t addSubPass(RenderPass::SubPass &&subpass);
		std::size_t addAttachment(ColorFormat format);

		void                setExtent(uvec2 &&extent) noexcept;
		inline const uvec2 &extent() const noexcept;

		inline const std::vector<ColorFormat> &attachments() const noexcept;

		inline bool isBuilt() const noexcept;
		inline bool hasDepthAttachment() const noexcept;

		inline const vk::RenderPass &vkRenderPass() const noexcept;

	private:
		void createRenderPass();
		void createFramebuffer();

		bool m_is_built;

		UniqueHandle<vk::RenderPass> m_render_pass;

		std::vector<RenderPass::SubPass> m_subpasses;
		std::vector<ColorFormat>         m_attachments;
		uvec2                            m_extent;

		bool m_has_depth_attachment;

		const Device &m_device;
	};
}

#include "RenderPassImpl.inl"
