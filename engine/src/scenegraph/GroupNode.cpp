// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/scenegraph/DirtyDefs.hpp>
#include <storm/engine/scenegraph/GroupNode.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
GroupNode::GroupNode(Scene &graph) : SceneNode {graph, "GroupNode"} {}

/////////////////////////////////////
/////////////////////////////////////
GroupNode::~GroupNode() = default;

/////////////////////////////////////
/////////////////////////////////////
GroupNode::GroupNode(GroupNode &&) = default;

/////////////////////////////////////
/////////////////////////////////////
GroupNode &GroupNode::operator=(GroupNode &&) = default;

/////////////////////////////////////
/////////////////////////////////////
SceneNode::DirtyType GroupNode::dirtyValue() const noexcept {
	return GROUP_BITS;
}
