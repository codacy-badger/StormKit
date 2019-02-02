#pragma once

#include <cstdint>

namespace storm::engine {
	constexpr static const std::uint32_t TRANSFORM_BITS           = 0b1;
	constexpr static const std::uint32_t VIEW_BITS                = 0b10;
	constexpr static const std::uint32_t SCENE_BITS               = 0b11;
	constexpr static const std::uint32_t PROGRAM_BITS             = 0b101;
	constexpr static const std::uint32_t MATERIAL_BITS            = 0b110;
	constexpr static const std::uint32_t GEOMETRY_BITS            = 0b111;
	constexpr static const std::uint32_t GEOMETRY_INSTANCED_BITS  = 0b1000;
	constexpr static const std::uint32_t DIRECTIONAL_LIGHT_BITS   = 0b1001;
	constexpr static const std::uint32_t GROUP_BITS               = 0b1010;
	constexpr static const std::uint32_t NODE_TYPE_MASK           = 0b00001111;

	constexpr static const std::uint32_t CREATE_NODE_STATE    = 0b00010000;
	constexpr static const std::uint32_t UPDATE_NODE_STATE    = 0b00100000;
	constexpr static const std::uint32_t AGGREGATE_NODE_STATE = 0b00110000;
	constexpr static const std::uint32_t ACTION_TYPE_MASK     = 0b11110000;
}
