// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/vulkan/FenceImpl.hpp>

namespace storm::engine {
	inline const vk::Fence &FenceImpl::vkFence() const noexcept {
		return m_fence.get();
	}
}
