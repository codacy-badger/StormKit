// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/Types.hpp>
#include <storm/engine/render/Shader.hpp>
#include <variant>

namespace storm::engine {
    class UniformBuffer;
    class Texture;
    struct UniformBufferBinding {
        const HardwareBuffer *buffer;
        std::uint32_t binding;
        std::size_t size;
        std::ptrdiff_t offset;
        Shader::Stage stages;
    };

    struct TextureBinding {
        const Texture *texture;
        std::uint32_t binding;
    };

    using Binding = std::variant<UniformBufferBinding, TextureBinding>;

    struct BindingState {
        std::vector<Binding> bindings;
    };
} // namespace storm::engine
