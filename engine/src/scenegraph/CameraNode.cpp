// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/scenegraph/CameraNode.hpp>
#include <storm/engine/scenegraph/DirtyDefs.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
CameraNode::CameraNode(Scene &graph)
      : SceneNode{graph, "CameraNode"}, m_camera{mat4{1.f}, mat4{1.f}, true, true} {
}

/////////////////////////////////////
/////////////////////////////////////
CameraNode::~CameraNode() = default;

/////////////////////////////////////
/////////////////////////////////////
CameraNode::CameraNode(CameraNode &&) = default;

/////////////////////////////////////
/////////////////////////////////////
CameraNode &CameraNode::operator=(CameraNode &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void CameraNode::commit() noexcept {
    notify(NodeEvent::UPDATED);
}

/////////////////////////////////////
/////////////////////////////////////
SceneNode::DirtyType CameraNode::dirtyValue() const noexcept {
    return CAMERA_BITS;
}
