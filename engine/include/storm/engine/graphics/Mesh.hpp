// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <optional>
#include <storm/engine/graphics/Material.hpp>
#include <storm/engine/render/HardwareBuffer.hpp>
#include <storm/engine/render/VertexInputState.hpp>
#include <storm/image/Image.hpp>

namespace storm::engine {
	struct MeshInstance {
		HardwareBuffer::Ptr vertex_buffer;
		std::size_t       vertex_count = 0u;
		VertexInputState  vertex_state;

		HardwareBuffer::Ptr index_buffer;
		std::size_t      index_count   = 0u;
		bool             large_indices = false;

		Material material;
		// Transform transform;

		mat4 projection;
		mat4 view;
		mat4 model;
		mat4 inverted_transform;

		std::uint32_t id;
	};
}
