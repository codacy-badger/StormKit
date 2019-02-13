// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/Filesystem.hpp>
#include <storm/core/Memory.hpp>
#include <storm/core/NonCopyable.hpp>
#include <storm/engine/graphics/MeshList.hpp>
#include <storm/engine/graphics/RenderGraph.hpp>
#include <storm/engine/graphics/ShaderLibrary.hpp>
#include <storm/engine/render/Fence.hpp>
#include <storm/engine/render/Semaphore.hpp>
#include <storm/engine/scenegraph/GroupNode.hpp>
#include <storm/engine/scenegraph/SceneTree.hpp>

namespace storm::engine {
	class Scene : public core::NonCopyable {
	public:
		SUR_Object(Scene)

		    explicit Scene(const Device &device);
		~Scene();

		Scene(Scene &&);
		Scene &operator=(Scene &&);

		inline GroupNode &      root() noexcept;
		inline const GroupNode &root() const noexcept;

		template <typename T, typename... Args>
		T &createNode(Args &&... arg);

		void update();

		void exportSceneGraph(const _std::filesystem::path &filepath) const;
		void exportSceneTree(const _std::filesystem::path &filepath) const;

		inline const MeshList &meshes() const noexcept;

	private:
		std::reference_wrapper<const Device> m_device;

		GroupNode                   m_root;
		std::vector<SceneNode::Ptr> m_nodes;

		SceneTree m_tree;

		MeshList m_cache;
	};

	template <typename T>
	constexpr std::string_view nameOfNode() {
		if constexpr (std::is_same_v<GroupNode, T>)
			return "GroupNode";
		else if constexpr (std::is_same_v<CameraNode, T>)
			return "CameraNode";
		else if constexpr (std::is_same_v<GeometryNode, T>)
			return "GeometryNode";
		else if constexpr (std::is_same_v<TransformNode, T>)
			return "TransformNode";
		else
			return "Unknown Node";
	}
}

#include "Scene.inl"
#include "Scene.tpp"
