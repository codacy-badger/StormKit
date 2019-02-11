// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/render/VertexInputState.hpp>
#include <storm/engine/render/VertexBuffer.hpp>
#include <storm/engine/render/IndexBuffer.hpp>

#include <storm/engine/graphics/Material.hpp>

#include <storm/image/Image.hpp>

#include <optional>

namespace storm::engine {
	struct MeshInstance {
		VertexBuffer::Ptr vertex_buffer;
		std::size_t       vertex_count = 0u;
		VertexInputState  vertex_state;

		IndexBuffer::Ptr index_buffer;
		std::size_t index_count = 0u;
		bool large_indices = false;

		Material  material;
		//Transform transform;

		mat4 projection;
		mat4 view;
		mat4 model;
		mat4 inverted_transform;

		std::uint32_t id;
	};
}
