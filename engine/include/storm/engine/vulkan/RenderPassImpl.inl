// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "RenderPassImpl.hpp"

namespace storm::engine {
    inline void RenderPassImpl::setFramebuffer(Framebuffer &framebuffer) {
        m_framebuffer = &framebuffer;
    }
    inline const Framebuffer *RenderPassImpl::framebuffer() const noexcept { return m_framebuffer; }

    inline bool RenderPassImpl::isBuilt() const noexcept { return m_is_built; }

    inline const vk::RenderPass &RenderPassImpl::vkRenderPass() const noexcept {
        return m_render_pass.get();
    }
} // namespace storm::engine
