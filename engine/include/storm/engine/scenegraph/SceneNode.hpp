// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/NonCopyable.hpp>
#include <storm/core/Memory.hpp>

#include <storm/engine/scenegraph/ForwardDeclarations.hpp>

#include <storm/tools/Subject.hpp>

#include <storm/engine/scenegraph/NodeData.hpp>

namespace storm::engine {
	class SceneNode : public core::NonCopyable, public NodeSubject {
		public:
			SUR_Object(SceneNode)

			using Array = std::vector<std::reference_wrapper<SceneNode>>;
			using ID = std::uint64_t;

			virtual ~SceneNode();

			void addChild(SceneNode &node);
			void removeChild(SceneNode &node);

			SceneNode(SceneNode &&);
			SceneNode &operator=(SceneNode &&);

			template <typename T>
			T &childAs(ID id);

			inline std::string_view name() const noexcept;
			inline const SceneNode::Array &parents() const noexcept;
			inline const SceneNode::Array &children() const noexcept;
			inline ID id() const noexcept;

			void setObserver(NodeObserver::RawPtr observer);
		protected:
			using DirtyType = std::uint32_t;

			explicit SceneNode(Scene &graph, std::string_view name);

			void notify(NodeEvent event) noexcept;

			virtual DirtyType dirtyValue() const noexcept = 0;

			friend class Scene;
			friend class SceneTree;

		private:
			ID m_id;
			std::string_view m_name;
			std::reference_wrapper<Scene> m_graph;

			SceneNode::Array m_parents;
			SceneNode::Array m_children;

			static inline auto next_id = 0u;
	};
}

#include "SceneNode.tpp"
#include "SceneNode.inl"
