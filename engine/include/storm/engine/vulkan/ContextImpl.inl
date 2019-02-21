// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/vulkan/ContextImpl.hpp>

namespace storm::engine {
    inline const ContextSettings &ContextImpl::settings() const noexcept {
        return m_context_settings;
    }

    inline const vk::DispatchLoaderDynamic &ContextImpl::dispatcher() const noexcept {
        return m_dispatcher;
    }

    inline const vk::Instance &ContextImpl::instance() const noexcept { return m_instance.get(); }
} // namespace storm::engine
