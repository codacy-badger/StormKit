// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/vulkan/FramebufferImpl.hpp>

namespace storm::engine {
	inline const vk::Framebuffer &FramebufferImpl::vkFramebuffer() const
	    noexcept {
		return m_framebuffer.get();
	}

	inline const std::vector<BackedVkImage> &
	FramebufferImpl::backedVkImages() const noexcept {
		return m_images;
	}
}
