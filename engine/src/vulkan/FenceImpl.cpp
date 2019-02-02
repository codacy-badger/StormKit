// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/vulkan/FenceImpl.hpp>
#include <storm/engine/vulkan/DeviceImpl.hpp>

#include <storm/log/LogOutput.hpp>

#include <array>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
FenceImpl::FenceImpl(const Device &device)
	: m_device{device} {
	const auto fence_create_info = vk::FenceCreateInfo{}
			.setFlags(vk::FenceCreateFlagBits::eSignaled);

	m_fence = m_device.implementation().vkDevice().createFenceUnique(fence_create_info);

	storm::DLOG("Renderer (vulkan)"_module, "Fence allocated at %{1}", &m_fence.get());
}

/////////////////////////////////////
/////////////////////////////////////
FenceImpl::~FenceImpl() = default;

/////////////////////////////////////
/////////////////////////////////////
FenceImpl::FenceImpl(FenceImpl &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void FenceImpl::wait(std::uint64_t timeout) {
	m_device.implementation().vkDevice().waitForFences({ m_fence.get() }, true, timeout);
}

/////////////////////////////////////
/////////////////////////////////////
void FenceImpl::reset() {
	m_device.implementation().vkDevice().resetFences({ m_fence.get() });
}
