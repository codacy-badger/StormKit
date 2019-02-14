// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/render/Framebuffer.hpp>
#include <storm/engine/vulkan/FramebufferImpl.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
Framebuffer::Framebuffer(
		const Device &device)
	: m_impl {device} {}

/////////////////////////////////////
/////////////////////////////////////
Framebuffer::~Framebuffer() = default;

/////////////////////////////////////
/////////////////////////////////////
Framebuffer::Framebuffer(Framebuffer &&) = default;

/////////////////////////////////////
/////////////////////////////////////
Framebuffer &Framebuffer::operator=(Framebuffer &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void Framebuffer::setExtent(uvec3 extent) {
	m_impl->setExtent(std::move(extent));
}

/////////////////////////////////////
/////////////////////////////////////
const uvec3 &Framebuffer::extent() const noexcept {
	return m_impl->extent();
}

/////////////////////////////////////
/////////////////////////////////////
uint32_t Framebuffer::addAttachment(Framebuffer::AttachmentDescription attachment) {
	return m_impl->addAttachment(std::move(attachment));
}

/////////////////////////////////////
/////////////////////////////////////
const Framebuffer::AttachmentDescriptions &Framebuffer::attachments() const noexcept {
	return m_impl->attachments();
}

/////////////////////////////////////
/////////////////////////////////////
bool Framebuffer::hasDepthAttachment() const noexcept {
	return m_impl->hasDepthAttachment();
}
