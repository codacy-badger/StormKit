// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/scenegraph/GeometryNode.hpp>
#include <storm/engine/scenegraph/DirtyDefs.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
GeometryNode::GeometryNode(Scene &graph)
	: SceneNode{graph, "GeometryNode"}
	{

}

/////////////////////////////////////
/////////////////////////////////////
GeometryNode::~GeometryNode() = default;

/////////////////////////////////////
/////////////////////////////////////
GeometryNode::GeometryNode(GeometryNode &&) = default;

/////////////////////////////////////
/////////////////////////////////////
GeometryNode &GeometryNode::operator=(GeometryNode &&) = default;

/////////////////////////////////////
/////////////////////////////////////
SceneNode::DirtyType GeometryNode::dirtyValue() const noexcept {
	return GEOMETRY_BITS;
}
