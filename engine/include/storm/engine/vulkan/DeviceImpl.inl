// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "DeviceImpl.hpp"

#include <storm/engine/vulkan/PhysicalDeviceImpl.hpp>

namespace storm::engine {
	inline const Context &DeviceImpl::context() const noexcept {
		return m_context;
	}

	inline const PhysicalDevice &DeviceImpl::physicalDevice() const noexcept {
		return m_physical_device;
	}

	inline DeviceImpl::FamilyQueueIndices DeviceImpl::familyQueuesIndices() const noexcept {
		return m_family_queue_indices;
	}
	
	inline PipelineCache &DeviceImpl::pipelineCache() noexcept {
		return m_pipeline_cache;
	}
	
	inline const PipelineCache &DeviceImpl::pipelineCache() const noexcept {
		return m_pipeline_cache;
	}

	inline const vk::Device &DeviceImpl::vkDevice() const noexcept {
		return m_logical_device.get();
	}

	inline const vk::Queue &DeviceImpl::vkGraphicsQueue() const noexcept {
		return m_graphics_queue;
	}

	inline const vk::Queue &DeviceImpl::vkComputeQueue() const noexcept {
		return m_compute_queue;
	}

	inline const vk::Queue &DeviceImpl::vkTransfertQueue() const noexcept {
		return m_transfert_queue;
	}
}
