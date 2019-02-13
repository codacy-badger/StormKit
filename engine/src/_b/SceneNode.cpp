#include "private/DirtyDefs.hpp"

#include <storm/engine/scenegraph/SceneNode.hpp>

using namespace storm::engine;

////////////////////////////////////////
////////////////////////////////////////
SceneNode::SceneNode() : Node(), m_projection(1.f) {}

////////////////////////////////////////
////////////////////////////////////////
uint32_t SceneNode::dirtyValue() const noexcept { return SCENE_BITS; }
