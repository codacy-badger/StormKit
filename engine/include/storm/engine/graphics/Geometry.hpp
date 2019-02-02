#pragma once

#include <storm/engine/graphics/Vertex.hpp>

#include <variant>

namespace storm::engine {
	struct Geometry {
		VertexArrayProxy vertices;
		IndexArrayProxy  indices;
	};
}
