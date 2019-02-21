// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/Memory.hpp>
#include <storm/core/NonCopyable.hpp>
#include <storm/core/Pimpl.hpp>
#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/Texture.hpp>
#include <storm/engine/render/Types.hpp>
#include <storm/engine/render/Utils.hpp>

namespace storm::engine {
    class FramebufferImpl;
    class Framebuffer {
    public:
        Unique_Object(Framebuffer)

          struct Attachment {
            ColorFormat format;
            uvec3 size;
            std::uint32_t mip_level;
        };

        enum class AttachmentType { INPUT, OUTPUT };

        using Attachments = std::vector<Attachment>;

        explicit Framebuffer(const Device &device);
        ~Framebuffer();

        Framebuffer(Framebuffer &&);
        Framebuffer &operator=(Framebuffer &&);

        void setExtent(uvec3 extent);
        const uvec3 &extent() const noexcept;

        std::uint32_t addInputAttachment(Attachment attachment, const Texture &texture);
        std::uint32_t addOutputAttachment(Attachment attachment);
        const std::vector<std::pair<Framebuffer::AttachmentType, Framebuffer::Attachment>> &
        attachments() const noexcept;

        bool hasDepthAttachment() const noexcept;

        IMPLEMENTATION(FramebufferImpl)
    private:
        core::Pimpl<FramebufferImpl> m_impl;
    };
} // namespace storm::engine
