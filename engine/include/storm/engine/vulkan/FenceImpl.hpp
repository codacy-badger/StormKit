// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/render/Fence.hpp>
#include <storm/engine/vulkan/UniqueHandle.hpp>

namespace storm::engine {
    class Device;
    class FenceImpl {
    public:
        explicit FenceImpl(const Device &device);
        ~FenceImpl();

        FenceImpl(FenceImpl &&);

        void wait(std::uint64_t timeout);
        void reset();

        inline const vk::Fence &vkFence() const noexcept;

    private:
        UniqueHandle<vk::Fence> m_fence;

        const Device &m_device;
    };
} // namespace storm::engine

#include "FenceImpl.inl"
