// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/render/RenderPass.hpp>
#include <storm/engine/vulkan/RenderPassImpl.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
RenderPass::RenderPass(const Device &device)
	: m_impl {device}
{}


/////////////////////////////////////
/////////////////////////////////////
RenderPass::~RenderPass() = default;

/////////////////////////////////////
/////////////////////////////////////
RenderPass::RenderPass(RenderPass &&) = default;

/////////////////////////////////////
/////////////////////////////////////
std::size_t RenderPass::addSubPass(RenderPass::SubPass &&subpass) {
	return m_impl->addSubPass(std::move(subpass));
}

/////////////////////////////////////
/////////////////////////////////////
void RenderPass::setFramebuffer(Framebuffer &framebuffer) {
	m_impl->setFramebuffer(framebuffer);
}

/////////////////////////////////////
/////////////////////////////////////
const Framebuffer *RenderPass::framebuffer() const noexcept {
	return m_impl->framebuffer();
}

/////////////////////////////////////
/////////////////////////////////////
void RenderPass::build() {
	m_impl->build();
}

/////////////////////////////////////
/////////////////////////////////////
bool RenderPass::isBuilt() const noexcept {
	return m_impl->isBuilt();
}
