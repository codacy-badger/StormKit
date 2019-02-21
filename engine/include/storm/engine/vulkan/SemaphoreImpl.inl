// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/vulkan/SemaphoreImpl.hpp>

namespace storm::engine {
    inline const vk::Semaphore &SemaphoreImpl::vkSemaphore() const noexcept {
        return m_semaphore.get();
    }
} // namespace storm::engine
