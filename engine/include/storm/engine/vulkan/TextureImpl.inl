// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/vulkan/TextureImpl.hpp>

namespace storm::engine {
    inline const Texture::Description &TextureImpl::description() const noexcept {
        return m_description;
    }

    inline const BackedVkTexture &TextureImpl::backedVkTexture() const noexcept {
        return m_texture;
    }
} // namespace storm::engine
