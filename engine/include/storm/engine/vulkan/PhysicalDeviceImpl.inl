// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/vulkan/PhysicalDeviceImpl.hpp>

namespace storm::engine {
    inline const vk::PhysicalDevice &PhysicalDeviceImpl::vkPhysicalDevice() const noexcept {
        return m_physical_device;
    }

    inline const vk::PhysicalDeviceProperties &
    PhysicalDeviceImpl::vkPhysicalDeviceProperties() const noexcept {
        return m_physical_device_properties;
    }

    inline const vk::PhysicalDeviceFeatures &PhysicalDeviceImpl::vkPhysicalDeviceFeatures() const
      noexcept {
        return m_physical_device_features;
    }

    inline const vk::PhysicalDeviceMemoryProperties &
    PhysicalDeviceImpl::vkPhysicalDeviceMemoryProperties() const noexcept {
        return m_physical_device_memory_properties;
    }
} // namespace storm::engine
