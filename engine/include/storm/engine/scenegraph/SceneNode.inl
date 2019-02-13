// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include "SceneNode.hpp"

namespace storm::engine {
	inline std::string_view SceneNode::name() const noexcept { return m_name; }

	inline const SceneNode::Array &SceneNode::parents() const noexcept {
		return m_parents;
	}

	inline const SceneNode::Array &SceneNode::children() const noexcept {
		return m_children;
	}

	inline SceneNode::ID SceneNode::id() const noexcept { return m_id; }
}
