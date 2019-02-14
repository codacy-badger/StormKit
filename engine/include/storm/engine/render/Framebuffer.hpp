// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/Memory.hpp>
#include <storm/core/NonCopyable.hpp>
#include <storm/core/Pimpl.hpp>

#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/Types.hpp>
#include <storm/engine/render/Utils.hpp>
#include <storm/engine/render/Texture.hpp>

namespace storm::engine {
	class FramebufferImpl;
	class Framebuffer {
	public:
		Unique_Object(Framebuffer)

		using AttachmentDescription  = Texture::Description;
		using AttachmentDescriptions = std::vector<AttachmentDescription>;

		explicit Framebuffer(const Device &device);
		~Framebuffer();

		Framebuffer(Framebuffer &&);
		Framebuffer &operator=(Framebuffer &&);

		void setExtent(uvec3 extent);
		const uvec3 &extent() const noexcept;

		std::uint32_t addAttachment(AttachmentDescription attachment);
		const AttachmentDescriptions &attachments() const noexcept;

		bool hasDepthAttachment() const noexcept;

		IMPLEMENTATION(FramebufferImpl)
	private:
		core::Pimpl<FramebufferImpl> m_impl;
	};
}
