// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <cstdint>

#include <storm/engine/render/PipelineState.hpp>

#include <storm/engine/graphics/Hashes.hpp>

namespace storm::engine {
	struct RendererCommandBufferCacheKey {
		std::size_t instance_count = 1;
		std::size_t index_count    = 0;
		std::size_t vertex_count   = 0;
		
		PipelineState state;
		
		bool operator==(const RendererCommandBufferCacheKey& key) {
			auto hasher = std::hash<RendererCommandBufferCacheKey>{};
			return hasher(key) == hasher(*this);
		}
	};
}
