// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <array>
#include <cstdint>
#include <initializer_list>

namespace storm::engine {
	static constexpr const auto FORWARD_RENDER_VERT_SRC = {
#include "shaders/forward_render.vert"
	};

	static constexpr const auto FORWARD_RENDER_FRAG_SRC = {
#include "shaders/forward_render.frag"
	};
}
