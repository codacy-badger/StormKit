// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/graphics/Vertex.hpp>

namespace storm::engine {
    struct Geometry {
        VertexArrayProxy vertices;
        IndexArrayProxy indices;
        VertexInputState vertex_state;
    };
} // namespace storm::engine
