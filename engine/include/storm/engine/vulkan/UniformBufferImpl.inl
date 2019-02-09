// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/vulkan/UniformBufferImpl.hpp>

namespace storm::engine {
	inline const UniformBuffer::Description &UniformBufferImpl::description() const noexcept {
		return m_description;
	}

	inline const BackedVkBuffer &UniformBufferImpl::backedVkBuffer() const noexcept {
		return m_buffer;
	}
}
