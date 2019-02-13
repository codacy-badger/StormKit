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

namespace storm::engine {
	class FramebufferImpl;
	class RenderPassImpl;
	class Framebuffer {
	public:
		Unique_Object(Framebuffer)

		    struct Description {};

		explicit Framebuffer(
		    const Device &device, const RenderPass &render_pass);
		explicit Framebuffer(
		    const Device &device, const RenderPassImpl &render_pass);
		~Framebuffer();

		Framebuffer(Framebuffer &&);
		Framebuffer &operator=(Framebuffer &&);

		IMPLEMENTATION(FramebufferImpl)
	private:
		core::Pimpl<FramebufferImpl> m_impl;
	};
}
