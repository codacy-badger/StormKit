// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/vulkan/FramebufferImpl.hpp>
#include <storm/engine/vulkan/TextureImpl.hpp>

namespace storm::engine {
    inline void FramebufferImpl::setExtent(uvec3 extent) { m_extent = std::move(extent); }

    inline const uvec3 &FramebufferImpl::extent() const noexcept { return m_extent; }

    inline std::uint32_t
    FramebufferImpl::addInputAttachment(Attachment attachment, const Texture &texture) {
        m_attachments.emplace_back(AttachmentType::INPUT, std::move(attachment));
        m_input_textures.emplace(std::size(m_attachments) - 1u, &texture);

        return std::size(m_attachments) - 1u;
    }

    inline std::uint32_t FramebufferImpl::addOutputAttachment(Attachment attachment) {
        m_attachments.emplace_back(AttachmentType::OUTPUT, std::move(attachment));

        return std::size(m_attachments) - 1u;
    }

    inline const std::vector<
      std::pair<FramebufferImpl::AttachmentType, FramebufferImpl::Attachment>> &
    FramebufferImpl::attachments() const noexcept {
        return m_attachments;
    }

    inline const vk::Framebuffer &FramebufferImpl::vkFramebuffer() const noexcept {
        return m_framebuffer.get();
    }

    inline const std::map<std::uint32_t, Texture> &FramebufferImpl::outputTextures() const
      noexcept {
        return m_output_textures;
    }
} // namespace storm::engine
