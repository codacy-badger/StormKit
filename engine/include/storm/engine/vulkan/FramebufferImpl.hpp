// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <map>
#include <storm/engine/render/Framebuffer.hpp>
#include <storm/engine/vulkan/DeviceImpl.hpp>
#include <storm/engine/vulkan/UniqueHandle.hpp>

namespace storm::engine {
    class FramebufferImpl {
    public:
        using AttachmentType = Framebuffer::AttachmentType;
        using Attachment     = Framebuffer::Attachment;
        using Attachments    = Framebuffer::Attachments;

        explicit FramebufferImpl(const Device &device);
        ~FramebufferImpl();

        FramebufferImpl(FramebufferImpl &&);

        inline void setExtent(uvec3 extent);
        inline const uvec3 &extent() const noexcept;

        inline std::uint32_t addInputAttachment(Attachment attachment, const Texture &texture);
        inline std::uint32_t addOutputAttachment(Attachment attachment);
        inline const std::vector<std::pair<AttachmentType, Attachment>> &attachments() const
          noexcept;

        bool hasDepthAttachment() const noexcept;

        inline const vk::Framebuffer &vkFramebuffer() const noexcept;
        inline const std::map<std::uint32_t, Texture> &outputTextures() const noexcept;

        void build(const vk::RenderPass &render_pass);

    private:
        void addAttachment(std::uint32_t id, const Framebuffer::Attachment &attachment);

        const Device &m_device;

        uvec3 m_extent;

        std::vector<std::pair<AttachmentType, Attachment>> m_attachments;
        std::map<std::uint32_t, const Texture *> m_input_textures;
        std::map<std::uint32_t, Texture> m_output_textures;

        UniqueHandle<vk::Framebuffer> m_framebuffer;
    };
} // namespace storm::engine

#include "FramebufferImpl.inl"
