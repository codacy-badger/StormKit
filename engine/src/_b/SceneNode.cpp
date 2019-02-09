#include <storm/engine/scenegraph/SceneNode.hpp>

#include "private/DirtyDefs.hpp"

using namespace storm::engine;

////////////////////////////////////////
////////////////////////////////////////
SceneNode::SceneNode()
	: Node(), m_projection(1.f) {

}

////////////////////////////////////////
////////////////////////////////////////
uint32_t SceneNode::dirtyValue() const noexcept {
	return SCENE_BITS;
}
