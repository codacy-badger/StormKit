// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/vulkan/FramebufferImpl.hpp>
#include <storm/engine/vulkan/TextureImpl.hpp>

namespace storm::engine {
	inline void FramebufferImpl::setExtent(uvec3 extent) {
		m_extent = std::move(extent);
	}

	inline const uvec3 &FramebufferImpl::extent() const noexcept {
		return m_extent;
	}

	inline std::uint32_t FramebufferImpl::addAttachment(AttachmentDescription attachment) {
		m_attachments.emplace_back(std::move(attachment));

		return std::size(m_attachments) - 1u;
	}

	inline const FramebufferImpl::AttachmentDescriptions &FramebufferImpl::attachments() const noexcept {
		return m_attachments;
	}

	inline const vk::Framebuffer &FramebufferImpl::vkFramebuffer() const
	    noexcept {
		return m_framebuffer.get();
	}

	inline const std::vector<BackedVkImage> &
	FramebufferImpl::backedVkImages() const noexcept {
		return m_backed_attachments;
	}
}
