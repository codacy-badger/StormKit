// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/render/Framebuffer.hpp>
#include <storm/engine/vulkan/FramebufferImpl.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
Framebuffer::Framebuffer(const Device &device, const RenderPassImpl &render_pass)
	: m_impl{device, render_pass} {

}

/////////////////////////////////////
/////////////////////////////////////
Framebuffer::~Framebuffer() = default;

/////////////////////////////////////
/////////////////////////////////////
Framebuffer::Framebuffer(Framebuffer &&) = default;

/////////////////////////////////////
/////////////////////////////////////
Framebuffer &Framebuffer::operator=(Framebuffer &&) = default;
