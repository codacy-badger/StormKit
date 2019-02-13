// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/vulkan/IndexBufferImpl.hpp>

namespace storm::engine {
	inline const BackedVkBuffer &IndexBufferImpl::backedVkBuffer() const
	    noexcept {
		return m_buffer;
	}
}
