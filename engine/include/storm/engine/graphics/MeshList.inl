// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "MeshList.hpp"

namespace storm::engine {
	inline std::pair<std::uint64_t, std::reference_wrapper<MeshInstance>> MeshList::addMesh() {
		auto &mesh = m_meshes.emplace_back();

		return std::make_pair(std::size(m_meshes) - 1, std::ref(mesh));
	}

	inline MeshInstance &MeshList::mesh(std::uint64_t id) {
		return m_meshes[id];
	}

	inline void MeshList::clear() {
		m_meshes.clear();
	}

	inline const MeshList::MeshVec &MeshList::meshes() const noexcept {
		return m_meshes;
	}
}
