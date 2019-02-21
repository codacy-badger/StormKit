// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "HardwareBufferImpl.hpp"

namespace storm::engine {
    inline const HardwareBuffer::Description &HardwareBufferImpl::description() {
        return m_description;
    }

    inline const BackedVkBuffer &HardwareBufferImpl::backedVkBuffer() const noexcept {
        return m_buffer;
    }
} // namespace storm::engine