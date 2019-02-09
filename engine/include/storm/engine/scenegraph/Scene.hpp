// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/NonCopyable.hpp>
#include <storm/core/Memory.hpp>
#include <storm/core/Filesystem.hpp>

#include <storm/engine/render/Fence.hpp>
#include <storm/engine/render/Semaphore.hpp>

#include <storm/engine/graphics/MeshList.hpp>
#include <storm/engine/graphics/RenderGraph.hpp>

#include <storm/engine/scenegraph/GroupNode.hpp>
#include <storm/engine/scenegraph/SceneTree.hpp>

namespace storm::engine {
	struct BeginTaskData;
	class Scene : public core::NonCopyable {
		public:
			SUR_Object(Scene)

			explicit Scene(const Device &device, const Surface &surface, uvec2 render_extent);
			~Scene();

			Scene(Scene &&);
			Scene &operator=(Scene &&);

			inline GroupNode &root() noexcept;
			inline const GroupNode &root() const noexcept;

			template <typename T, typename... Args>
			T &createNode(Args &&...arg);

			void update();

			void exportSceneGraph(const _std::filesystem::path &filepath) const;
			void exportSceneTree(const _std::filesystem::path &filepath) const;
			void exportRenderGraph(const _std::filesystem::path &filepath) const;
		private:
			void updateRenderGraph();
			void addDefaultForwardRenderTask(const BeginTaskData &begin_task_data);

			struct CameraMatrices {
				mat4 projection;
				mat4 view;
			};

			std::reference_wrapper<const Device> m_device;
			std::reference_wrapper<const Surface> m_surface;
			uvec2 m_render_extent;

			GroupNode m_root;
			std::vector<SceneNode::Ptr> m_nodes;

			Fence m_fence;
			Semaphore m_semaphore;

			SceneTree m_tree;
			RenderGraph m_render_graph;

			MeshList m_cache;

			Texture::Description m_backbuffer_desc;
			UniformBuffer::Description m_camera_buffer_desc;
			UniformBuffer::Ptr         m_camera_buffer;
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
