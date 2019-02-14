// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/render/Framebuffer.hpp>
#include <storm/engine/vulkan/DeviceImpl.hpp>
#include <storm/engine/vulkan/UniqueHandle.hpp>

namespace storm::engine {
	class FramebufferImpl {
	public:
		using AttachmentDescription    = Framebuffer::AttachmentDescription;
		using AttachmentDescriptions   = Framebuffer::AttachmentDescriptions;

		explicit FramebufferImpl(
			const Device &device);
		~FramebufferImpl();
	
		FramebufferImpl(FramebufferImpl &&);

		inline void setExtent(uvec3 extent);
		inline const uvec3 &extent() const noexcept;

		inline std::uint32_t addAttachment(AttachmentDescription attachment);
		inline const AttachmentDescriptions &attachments() const noexcept;

		bool hasDepthAttachment() const noexcept;

		inline const vk::Framebuffer &           vkFramebuffer() const noexcept;
		inline const std::vector<BackedVkImage> &backedVkImages() const
			noexcept;

		void build(const vk::RenderPass &render_pass);
	private:
		void addAttachment(std::uint8_t mip_level, ColorFormat format);

		const DeviceImpl &m_device;

		uvec3 m_extent;

		AttachmentDescriptions m_attachments;

		UniqueHandle<vk::Framebuffer> m_framebuffer;
		std::vector<BackedVkImage>    m_backed_attachments;
	};
}

#include "FramebufferImpl.inl"
