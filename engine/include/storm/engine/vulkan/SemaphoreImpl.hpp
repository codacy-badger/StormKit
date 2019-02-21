// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/render/Semaphore.hpp>
#include <storm/engine/vulkan/UniqueHandle.hpp>

namespace storm::engine {
    class Device;
    class SemaphoreImpl : core::NonCopyable {
    public:
        explicit SemaphoreImpl(const Device &device);
        ~SemaphoreImpl();

        SemaphoreImpl(SemaphoreImpl &&);

        inline const vk::Semaphore &vkSemaphore() const noexcept;

    private:
        UniqueHandle<vk::Semaphore> m_semaphore;

        const Device &m_device;
    };
} // namespace storm::engine

#include "SemaphoreImpl.inl"
