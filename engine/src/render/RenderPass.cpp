// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/render/RenderPass.hpp>
#include <storm/engine/vulkan/RenderPassImpl.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
RenderPass::RenderPass(const Device &device)
	: m_impl{device} {

}

/////////////////////////////////////
/////////////////////////////////////
RenderPass::~RenderPass() = default;

/////////////////////////////////////
/////////////////////////////////////
RenderPass::RenderPass(RenderPass &&) = default;

/////////////////////////////////////
/////////////////////////////////////
std::size_t RenderPass::addSubPass(SubPass subpass) {
	return m_impl->addSubPass(std::move(subpass));
}

/////////////////////////////////////
/////////////////////////////////////
std::size_t RenderPass::addAttachment(ColorFormat format) {
	return m_impl->addAttachment(format);
}

bool RenderPass::hasDepthAttachment() const noexcept {
	return m_impl->hasDepthAttachment();
}

/////////////////////////////////////
/////////////////////////////////////
void RenderPass::setExtent(uvec2 extent) {
	m_impl->setExtent(std::move(extent));
}

/////////////////////////////////////
/////////////////////////////////////
void RenderPass::build() {
	m_impl->init();
}
