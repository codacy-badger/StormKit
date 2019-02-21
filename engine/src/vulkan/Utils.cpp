// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/vulkan/Utils.hpp>

namespace storm::engine {
    bool isColorFormat(vk::Format format) {
        return !isDepthStencilFormat(format) && !isDepthOnlyFormat(format);
    }

    bool isDepthFormat(vk::Format format) {
        return isDepthStencilFormat(format) || isDepthOnlyFormat(format);
    }

    bool isDepthOnlyFormat(vk::Format format) {
        return format == vk::Format::eD16Unorm || format == vk::Format::eD32Sfloat;
    }

    bool isDepthStencilFormat(vk::Format format) {
        return format == vk::Format::eD16UnormS8Uint || format == vk::Format::eD24UnormS8Uint ||
               format == vk::Format::eD32SfloatS8Uint;
    }
} // namespace storm::engine
