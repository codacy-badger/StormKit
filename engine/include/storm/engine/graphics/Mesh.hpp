// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/graphics/Vertex.hpp>

#include <storm/image/Image.hpp>

#include <optional>

namespace storm::engine {
	struct Mesh {
		VertexArrayProxy vertices;
		IndexArrayProxy  indices;

		std::optional<std::reference_wrapper<image::Image>> diffuse = std::nullopt;
	};
}
