// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "ShaderImpl.hpp"

namespace storm::engine {
    inline Shader::Stage ShaderImpl::stage() const noexcept { return m_stage; }

    inline const std::vector<std::byte> &ShaderImpl::source() const noexcept { return m_source; }

    inline const vk::ShaderModule &ShaderImpl::vkShaderModule() const noexcept {
        return m_shader_module.get();
    }
} // namespace storm::engine
