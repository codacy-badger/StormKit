// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/vulkan/DeviceImpl.hpp>
#include <storm/engine/vulkan/SemaphoreImpl.hpp>
#include <storm/log/LogOutput.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
SemaphoreImpl::SemaphoreImpl(const Device &device) : m_device {device} {
	const auto semaphore_create_info = vk::SemaphoreCreateInfo {};

	m_semaphore = m_device.implementation().vkDevice().createSemaphoreUnique(
	    semaphore_create_info);

	storm::DLOG("Renderer (vulkan)"_module, "Semaphore allocated at %{1}",
	    &m_semaphore.get());
}

/////////////////////////////////////
/////////////////////////////////////
SemaphoreImpl::~SemaphoreImpl() = default;

/////////////////////////////////////
/////////////////////////////////////
SemaphoreImpl::SemaphoreImpl(SemaphoreImpl &&) = default;
