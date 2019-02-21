// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <optional>
#include <storm/core/Types.hpp>
#include <storm/engine/render/Types.hpp>
#include <storm/image/Image.hpp>

namespace storm::engine {
    struct Material {
        using Texture    = std::reference_wrapper<image::Image>;
        using TextureOpt = std::optional<Texture>;

        core::RGBColorF diffuse;
        core::RGBColorF ambiant;
        core::RGBColorF specular;

        TextureOpt diffuse_map  = std::nullopt;
        TextureOpt ambiant_map  = std::nullopt;
        TextureOpt specular_map = std::nullopt;
        TextureOpt normal_map   = std::nullopt;
    };
} // namespace storm::engine
