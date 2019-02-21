// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/Types.hpp>
#include <storm/engine/render/VertexInputState.hpp>

namespace storm::engine {
    struct PipelineState {
        bool is_blending_enabled                  = false;
        ColorComponentFlag blend_write_color_mask = ColorComponentFlag::RGBA;

        bool is_depth_test_enabled               = false;
        CompareOperation depth_compare_operation = CompareOperation::LESS;

        bool is_stencil_test_enabled = false;

        VertexInputState vertex_input_state;

        core::RGBColorF clear_color;

        PrimitiveTopology topology = PrimitiveTopology::TRIANGLES;

        Viewport viewport = {0.f, 0.f, 1.f, 1.f, 0.f, 1.f};

        ivec4 scissor = {};

        float line_width = 1.f;

        PolygonMode polygon_mode = PolygonMode::FILL;

        CullMode cull_mode = CullMode::BACK;

        FrontFace front_face = FrontFace::CLOCKWISE;
    };
} // namespace storm::engine
