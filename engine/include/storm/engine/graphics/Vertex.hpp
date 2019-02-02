#pragma once

#include <storm/engine/render/Types.hpp>

#include <storm/engine/render/VertexInputState.hpp>

#include <variant>

namespace storm::engine {
	struct Vertex_P {
		vec3 position;
	};

	struct Vertex_P_C {
		vec3 position;
		vec4 color;
	};
	
	struct Vertex_P_C_N {
		vec3 position;
		vec4 color;
		vec3 normal;
	};
	
	struct Vertex_P_C_UV {
		vec3 position;
		vec4 color;
		vec2 uv;
	};

	struct Vertex_P_UV {
		vec3 position;
		vec2 uv;
	};

	struct Vertex_P_UV_N {
		vec3 position;
		vec2 uv;
		vec3 normal;
	};

	struct Vertex_P_UV_N_T_BT {
		vec3 position;
		vec2 uv;
		vec3 normal;
		vec3 tangent;
		vec3 bitangent;
	};

	static inline const auto VERTEX_P = VertexInputAttributes{
		{0, engine::Format::FLOAT_VEC3, 0, offsetof(Vertex_P, position)}
	};
	static inline const auto VERTEX_P_C = VertexInputAttributes{
		{0, engine::Format::FLOAT_VEC3, 0, offsetof(Vertex_P_C, position)},
		{0, engine::Format::FLOAT_VEC4, 1, offsetof(Vertex_P_C, color)}
	};
	static inline const auto VERTEX_P_C_N = VertexInputAttributes{
		{0, engine::Format::FLOAT_VEC3, 0, offsetof(Vertex_P_C_N, position)},
		{0, engine::Format::FLOAT_VEC4, 1, offsetof(Vertex_P_C_N, color)},
		{0, engine::Format::FLOAT_VEC3, 2, offsetof(Vertex_P_C_N, normal)}
	};
	static inline const auto VERTEX_P_C_UV = VertexInputAttributes{
		{0, engine::Format::FLOAT_VEC3, 0, offsetof(Vertex_P_C_UV, position)},
		{0, engine::Format::FLOAT_VEC4, 1, offsetof(Vertex_P_C_UV, color)},
		{0, engine::Format::FLOAT_VEC2, 2, offsetof(Vertex_P_C_UV, uv)}
	};
	
	using IndexArray = std::vector<std::uint16_t>;
	using LargeIndexArray = std::vector<std::uint32_t>;
	
	template <typename T = Vertex_P_UV_N>
	using VertexArray = std::vector<T>;
	
	using IndexArrayProxy = std::variant<IndexArray, LargeIndexArray>;
	using VertexArrayProxy = std::variant<
		VertexArray<Vertex_P>,
		VertexArray<Vertex_P_C>,
		VertexArray<Vertex_P_C_N>,
		VertexArray<Vertex_P_C_UV>
	>;
}
