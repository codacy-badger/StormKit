// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/NonCopyable.hpp>
#include <storm/engine/graphics/Mesh.hpp>

namespace storm::engine {
	class MeshList : public core::NonCopyable {
	public:
		using MeshVec = std::vector<MeshInstance>;
		explicit MeshList();
		~MeshList();

		MeshList(MeshList &&);
		MeshList &operator=(MeshList &&);

		inline std::pair<std::uint64_t, std::reference_wrapper<MeshInstance>>
		                     addMesh();
		inline MeshInstance &mesh(std::uint64_t id);

		inline void clear();

		inline const MeshVec &meshes() const noexcept;

	private:
		MeshVec m_meshes;
	};
}

#include "MeshList.inl"
