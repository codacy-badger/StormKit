// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "RenderPassImpl.hpp"

namespace storm::engine {
	inline const uvec2 &RenderPassImpl::extent() const noexcept {
		return m_extent;
	}

	inline const std::vector<ColorFormat> &RenderPassImpl::attachments() const noexcept {
		return m_attachments;
	}

	inline bool RenderPassImpl::isBuilt() const noexcept {
		return m_is_built;
	}
	
	inline bool RenderPassImpl::hasDepthAttachment() const noexcept {
		return m_has_depth_attachment;
	}

	inline const vk::RenderPass &RenderPassImpl::vkRenderPass() const noexcept {
		return m_render_pass.get();
	}
}
