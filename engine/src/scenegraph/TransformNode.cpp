// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/scenegraph/DirtyDefs.hpp>
#include <storm/engine/scenegraph/TransformNode.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
TransformNode::TransformNode(Scene &graph)
    : SceneNode {graph, "TransformNode"} {}

/////////////////////////////////////
/////////////////////////////////////
TransformNode::~TransformNode() = default;

/////////////////////////////////////
/////////////////////////////////////
TransformNode::TransformNode(TransformNode &&) = default;

/////////////////////////////////////
/////////////////////////////////////
TransformNode &TransformNode::operator=(TransformNode &&) = default;

/////////////////////////////////////
/////////////////////////////////////
SceneNode::DirtyType TransformNode::dirtyValue() const noexcept {
	return TRANSFORM_BITS;
}
