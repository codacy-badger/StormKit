#include <storm/engine/scenegraph/DirectionalLightNode.hpp>
#include "private/DirtyDefs.hpp"

using namespace storm::engine;

DirectionalLightNode::DirectionalLightNode()
	: Node() {

}

std::uint32_t DirectionalLightNode::dirtyValue() const noexcept {
	return DIRECTIONAL_LIGHT_BITS;
}
