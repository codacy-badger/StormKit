#pragma once

#include "CommandBuffer.hpp"

namespace storm::engine {
	inline const CommandBufferProperties &CommandBuffer::properties() const
	    noexcept {
		return m_properties;
	}
}
