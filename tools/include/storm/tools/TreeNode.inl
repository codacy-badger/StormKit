// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "TreeNode.hpp"

namespace storm::tools {
	inline const std::string &TreeNode::name() const noexcept { return m_name; }

	inline void TreeNode::setName(std::string_view name) noexcept {
		m_name = name;
	}

	inline TreeNode::Index TreeNode::parent() const noexcept {
		return m_parent;
	}

	inline void TreeNode::setParent(Index index) noexcept { m_parent = index; }

	inline TreeNode::Index TreeNode::nextSibling() const noexcept {
		return m_next_sibling;
	}

	inline void TreeNode::setNextSibling(Index index) noexcept {
		m_next_sibling = index;
	}

	inline TreeNode::Index TreeNode::firstChild() const noexcept {
		return m_first_child;
	}

	inline void TreeNode::setFirstChild(Index index) noexcept {
		m_first_child = index;
	}

	inline const TreeNode::DirtyBits &TreeNode::dirtyBits() const noexcept {
		return m_dirty_bits;
	}

	inline void TreeNode::setDirtyBits(DirtyBits bits) noexcept {
		m_dirty_bits = bits;
	}
}
