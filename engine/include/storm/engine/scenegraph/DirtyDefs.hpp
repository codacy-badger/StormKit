#pragma once

#include <cstdint>

namespace storm::engine {
	constexpr static const std::uint32_t TRANSFORM_BITS = 0b1;
	constexpr static const std::uint32_t CAMERA_BITS    = 0b10;
	constexpr static const std::uint32_t SCENE_BITS     = 0b11;
	constexpr static const std::uint32_t GEOMETRY_BITS  = 0b111;
	constexpr static const std::uint32_t GROUP_BITS     = 0b1010;
	constexpr static const std::uint32_t NODE_TYPE_MASK = 0b00001111;

	constexpr static const std::uint32_t CREATE_NODE_STATE = 0b00010000;
	constexpr static const std::uint32_t UPDATE_NODE_STATE = 0b00100000;
	constexpr static const std::uint32_t ACTION_TYPE_MASK  = 0b11110000;
}
