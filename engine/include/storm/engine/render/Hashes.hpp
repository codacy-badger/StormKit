// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/Hash.hpp>

#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/Types.hpp>

#include <functional>

#include <glm/gtx/hash.hpp>

namespace std {
	HASH_FUNC(storm::engine::PipelineState)
	HASH_FUNC(storm::engine::VertexInputState)
	HASH_FUNC(storm::engine::VertexInputAttribute)
	HASH_FUNC(storm::engine::VertexInputBinding)
	HASH_FUNC(storm::engine::UniformBufferBinding)
	HASH_FUNC(storm::engine::TextureBinding)
	HASH_FUNC(storm::engine::BindingState)
}
