// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/Flags.hpp>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

namespace storm::engine {
	using vec2  = glm::vec2;
	using vec3  = glm::vec3;
	using vec4  = glm::vec4;
	using uvec2  = glm::uvec2;
	using uvec3  = glm::uvec3;
	using uvec4  = glm::uvec4;
	using ivec2  = glm::ivec2;
	using ivec3  = glm::ivec3;
	using ivec4  = glm::ivec4;
	using mat4  = glm::mat4;
	using quat  = glm::quat;

	struct Viewport {
		float top;
		float left;
		float width;
		float height;
		float min_depth = 0.f;
		float max_depth = 1.f;
	};

	enum class ColorComponent {
		R = 1,
		G = 2,
		B = 4,
		A = 8,
		RGBA = R | G | B | A,
		SIZE
	};

	enum class PrimitiveTopology {
		TRIANGLES,
		TRIANGLE_STRIP,
		SIZE
	};

	enum class PolygonMode {
		POINT,
		LINE,
		FILL,
		SIZE
	};

	enum class CullMode {
		FRONT,
		BACK,
		FRONT_AND_BACK,
		NONE,
		SIZE
	};

	enum class FrontFace {
		CLOCKWISE,
		COUNTER_CLOCKWISE,
		SIZE
	};

	enum class Format {
		BOOL,
		BOOL_VEC2,
		BOOL_VEC3,
		BOOL_VEC4,
		BYTE,
		INT,
		INT_VEC2,
		INT_VEC3,
		INT_VEC4,
		INT_SAMPLER_BUFFER,
		SHORT,
		UBYTE,
		UINT,
		UINT_VEC2,
		UINT_VEC3,
		UINT_VEC4,
		USHORT,
		FLOAT,
		FLOAT_VEC2,
		FLOAT_VEC3,
		FLOAT_VEC4,
		FLOAT_MAT2,
		FLOAT_MAT3,
		FLOAT_MAT4,

#ifndef STORM_OS_IOS
		DOUBLE,
		DOUBLE_VEC2,
		DOUBLE_VEC3,
		DOUBLE_VEC4,
		SAMPLER_1D,
#endif
		SAMPLER_2D,
		SAMPLER_3D,
		SAMPLER_BUFFER,
		RGBA8888UNORM,
		RGBA8888SNORM,
		RGBA8888SRGB,
		BGRA8888UNORM,
		BGRA8888SNORM,
		BGRA8888SRGB,
		D32,
		D32S8,
		D24S8UNORM,
		SIZE
	};

	enum class VertexInputRate {
		PER_VERTEX,
		PER_INSTANCE,
		SIZE
	};

	enum class PipelineStage {
		COLOR_ATTACHMENT_OUTPUT,
		SIZE
	};
	
	enum class CompareOperation {
		NEVER,
		LESS,
		EQUAL,
		LESS_OR_EQUAL,
		GREATER,
		NOT_EQUAL,
		GREATER_OR_EQUAL,
		ALWAYS,
		SIZE
	};
	
	using ColorFormat = Format;
}

FLAG_ENUM(storm::engine::ColorComponent)
