// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "Scene.hpp"

namespace storm::engine {
	inline GroupNode &Scene::root() noexcept { return m_root; }

	inline const GroupNode &Scene::root() const noexcept { return m_root; }

	inline const MeshList &Scene::meshes() const noexcept { return m_cache; }
}
