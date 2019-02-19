// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <functional>
#include <storm/core/Hash.hpp>
#include <storm/engine/graphics/ForwardDeclarations.hpp>

#include <glm/gtx/hash.hpp>

namespace std {
	/*HASH_FUNC(storm::engine::Material)
	HASH_FUNC(storm::engine::Drawable)
	HASH_FUNC(storm::engine::DirectionalLight)
	HASH_FUNC(storm::engine::Geometry)
	HASH_FUNC(storm::engine::Transform)*/
	HASH_FUNC(storm::engine::Vertex_P)
	HASH_FUNC(storm::engine::Vertex_P_C)
	HASH_FUNC(storm::engine::Vertex_P_C_N)
	HASH_FUNC(storm::engine::Vertex_P_C_UV)
	HASH_FUNC(storm::engine::Vertex_P_UV)
	HASH_FUNC(storm::engine::Vertex_P_UV_N)
	HASH_FUNC(storm::engine::Vertex_P_UV_N_T_BT)
	// HASH_FUNC(storm::engine::RendererCommandBufferCacheKey)
}
