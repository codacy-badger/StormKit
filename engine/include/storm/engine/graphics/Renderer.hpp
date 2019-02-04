// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/NonCopyable.hpp>
#include <storm/core/Memory.hpp>

namespace storm::engine {
	enum class RenderMethod {
		FORWARD_RENDERING,
		DEFERRED_RENDERING
	};

	template <auto Method>
	class Renderer;
}
