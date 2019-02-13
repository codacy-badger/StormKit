// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "ResourcePool.hpp"

namespace storm::engine {
	inline const std::vector<TextureResource::Ptr> &
	ResourcePool::textures() const noexcept {
		return m_textures;
	}

	inline const std::vector<UniformBufferResource::Ptr> &
	ResourcePool::buffers() const noexcept {
		return m_buffers;
	}
}
