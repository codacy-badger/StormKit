// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <vector>
#include <cstdint>

#include <storm/engine/render/Types.hpp>

namespace storm::engine {
	struct VertexInputAttribute {
		std::uint32_t binding;
		Format        type;
		std::uint32_t location;
		std::size_t   offset;
	};

	struct VertexInputBinding {
		std::uint32_t   binding;
		std::size_t     stride;
		VertexInputRate rate;
	};

	using VertexInputAttributes = std::vector<VertexInputAttribute>;
	using VertexInputBindings = std::vector<VertexInputBinding>;

	struct VertexInputState {
		VertexInputAttributes attributes;
		VertexInputBindings   bindings;
	};
}
