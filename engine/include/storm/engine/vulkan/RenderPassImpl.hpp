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

        std::size_t addSubPass(RenderPass::SubPass &&subpass);

        inline void setFramebuffer(Framebuffer &framebuffer);
        inline const Framebuffer *framebuffer() const noexcept;

        void build();
        inline bool isBuilt() const noexcept;

        inline const vk::RenderPass &vkRenderPass() const noexcept;

    private:
        void createRenderPass();

        const Device &m_device;

        Framebuffer *m_framebuffer;

        bool m_is_built;

        std::vector<RenderPass::SubPass> m_subpasses;

        UniqueHandle<vk::RenderPass> m_render_pass;
    };
} // namespace storm::engine

#include "RenderPassImpl.inl"
