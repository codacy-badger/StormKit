// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <vulkan/vulkan.hpp>

namespace storm::engine {
	bool isColorFormat(vk::Format format);
	bool isDepthFormat(vk::Format format);
	bool isDepthOnlyFormat(vk::Format format);
	bool isDepthStencilFormat(vk::Format format);
}
