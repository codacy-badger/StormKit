// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/graphics/Renderer.hpp>

namespace storm::engine {
	template <typename T>
	void Renderer::addGraphUpdateFunction(RenderMethod method, T &&func) {
		m_graph_update_funcs.emplace(method, std::forward<T>(func));
	}
}
