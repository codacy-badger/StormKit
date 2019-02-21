// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/NonCopyable.hpp>
#include <storm/engine/vulkan/UniqueHandle.hpp>

namespace storm::engine {
    class PhysicalDeviceImpl : public core::NonCopyable {
    public:
        explicit PhysicalDeviceImpl(vk::PhysicalDevice device);
        ~PhysicalDeviceImpl();

        PhysicalDeviceImpl(PhysicalDeviceImpl &&);
        PhysicalDeviceImpl &operator=(PhysicalDeviceImpl &&);

        inline const vk::PhysicalDevice &vkPhysicalDevice() const noexcept;
        inline const vk::PhysicalDeviceProperties &vkPhysicalDeviceProperties() const noexcept;
        inline const vk::PhysicalDeviceFeatures &vkPhysicalDeviceFeatures() const noexcept;
        inline const vk::PhysicalDeviceMemoryProperties &vkPhysicalDeviceMemoryProperties() const
          noexcept;

    private:
        vk::PhysicalDevice m_physical_device;
        vk::PhysicalDeviceProperties m_physical_device_properties;
        vk::PhysicalDeviceFeatures m_physical_device_features;
        vk::PhysicalDeviceMemoryProperties m_physical_device_memory_properties;
    };
} // namespace storm::engine

#include "PhysicalDeviceImpl.inl"
