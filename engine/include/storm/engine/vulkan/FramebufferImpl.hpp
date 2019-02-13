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
		explicit FramebufferImpl(
			const Device &device, const RenderPass &render_pass);
		explicit FramebufferImpl(
		const Device &device, const RenderPassImpl &render_pass);
		~FramebufferImpl();
	
		FramebufferImpl(FramebufferImpl &&);
	
		inline const vk::Framebuffer &           vkFramebuffer() const noexcept;
		inline const std::vector<BackedVkImage> &backedVkImages() const
			noexcept;
	
	private:
		void createFramebuffer(bool build_from_render_pass);
	
		UniqueHandle<vk::Framebuffer> m_framebuffer;
		std::vector<BackedVkImage>    m_attachments;
	
		const DeviceImpl &    m_device;
		const RenderPassImpl &m_render_pass;
	};
}

#include "FramebufferImpl.inl"
