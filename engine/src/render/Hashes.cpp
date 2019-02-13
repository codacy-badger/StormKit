// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/core/Numerics.hpp>
#include <storm/engine/render/Hashes.hpp>
#include <storm/engine/render/PipelineState.hpp>
#include <storm/engine/render/VertexInputState.hpp>

using namespace storm;
using namespace storm::core;
using namespace storm::engine;

static auto float_hasher              = std::hash<float> {};
static auto bool_hasher               = std::hash<bool> {};
static auto vec2_hasher               = std::hash<vec2> {};
static auto vec3_hasher               = std::hash<vec3> {};
static auto vec4_hasher               = std::hash<vec4> {};
static auto mat4_hasher               = std::hash<mat4> {};
static auto quat_hasher               = std::hash<quat> {};
static auto color_component_hasher    = std::hash<ColorComponent> {};
static auto compare_op_hasher         = std::hash<CompareOperation> {};
static auto vertex_input_state_hasher = std::hash<VertexInputState> {};
static auto color_hasher              = std::hash<RGBColorF> {};
static auto primitive_topology_hasher = std::hash<PrimitiveTopology> {};
static auto polygon_mode_hasher       = std::hash<PolygonMode> {};
static auto cull_mode_hasher          = std::hash<CullMode> {};
static auto front_face_hasher         = std::hash<FrontFace> {};
static auto vertex_attribute_hasher   = std::hash<VertexInputAttribute> {};
static auto vertex_attributes_hasher  = std::hash<VertexInputAttributes> {};
static auto vertex_bindings_hasher    = std::hash<VertexInputBindings> {};
static auto vertex_binding_hasher     = std::hash<VertexInputBinding> {};
static auto format_hasher             = std::hash<Format> {};
static auto size_t_hasher             = std::hash<std::size_t> {};
static auto uint32_t_hasher           = std::hash<std::uint32_t> {};
static auto vertex_input_rate_hasher  = std::hash<VertexInputRate> {};
static auto pipeline_state_hasher     = std::hash<PipelineState> {};

namespace std {
	std::size_t hash<PipelineState>::operator()(
	    const PipelineState &pipeline) const {
		using namespace storm::core;
		using namespace storm::engine;

		auto hash = bool_hasher(pipeline.is_blending_enabled);
		hash_combine(
		    hash, color_component_hasher(pipeline.blend_write_color_mask));
		hash_combine(hash, bool_hasher(pipeline.is_depth_test_enabled));
		hash_combine(hash, compare_op_hasher(pipeline.depth_compare_operation));
		hash_combine(hash, bool_hasher(pipeline.is_stencil_test_enabled));
		hash_combine(
		    hash, vertex_input_state_hasher(pipeline.vertex_input_state));
		hash_combine(hash, color_hasher(pipeline.clear_color));
		hash_combine(hash, primitive_topology_hasher(pipeline.topology));
		hash_combine(hash, float_hasher(pipeline.viewport.width));
		hash_combine(hash, float_hasher(pipeline.viewport.height));
		hash_combine(hash, float_hasher(pipeline.viewport.top));
		hash_combine(hash, float_hasher(pipeline.viewport.left));
		hash_combine(hash, float_hasher(pipeline.viewport.max_depth));
		hash_combine(hash, float_hasher(pipeline.viewport.min_depth));
		hash_combine(hash, float_hasher(pipeline.scissor.x));
		hash_combine(hash, float_hasher(pipeline.scissor.y));
		hash_combine(hash, float_hasher(pipeline.scissor.z));
		hash_combine(hash, float_hasher(pipeline.scissor.w));
		hash_combine(hash, float_hasher(pipeline.line_width));
		hash_combine(hash, polygon_mode_hasher(pipeline.polygon_mode));
		hash_combine(hash, cull_mode_hasher(pipeline.cull_mode));
		hash_combine(hash, front_face_hasher(pipeline.front_face));

		return hash;
	}

	bool equal_to<PipelineState>::operator()(const PipelineState &first,
	    const PipelineState &second) const noexcept {
		return pipeline_state_hasher(first) == pipeline_state_hasher(second);
	}

	std::size_t hash<VertexInputState>::operator()(
	    const VertexInputState &obj) const {
		auto hash = vertex_attributes_hasher(obj.attributes);
		hash_combine(hash, vertex_bindings_hasher(obj.bindings));

		return hash;
	}

	bool equal_to<VertexInputState>::operator()(const VertexInputState &first,
	    const VertexInputState &second) const noexcept {
		return vertex_input_state_hasher(first)
		       == vertex_input_state_hasher(second);
	}

	std::size_t hash<VertexInputAttribute>::operator()(
	    const VertexInputAttribute &obj) const {
		auto hash = format_hasher(obj.type);
		hash_combine(hash, size_t_hasher(obj.offset));
		hash_combine(hash, uint32_t_hasher(obj.binding));
		hash_combine(hash, uint32_t_hasher(obj.location));

		return hash;
	}

	bool equal_to<VertexInputAttribute>::operator()(
	    const VertexInputAttribute &first,
	    const VertexInputAttribute &second) const noexcept {
		return vertex_attribute_hasher(first)
		       == vertex_attribute_hasher(second);
	}

	std::size_t hash<VertexInputBinding>::operator()(
	    const VertexInputBinding &obj) const {
		auto hash = vertex_input_rate_hasher(obj.rate);
		hash_combine(hash, size_t_hasher(obj.stride));
		hash_combine(hash, uint32_t_hasher(obj.binding));

		return hash;
	}

	bool equal_to<VertexInputBinding>::operator()(
	    const VertexInputBinding &first, const VertexInputBinding &second) const
	    noexcept {
		return vertex_binding_hasher(first) == vertex_binding_hasher(second);
	}
}
