#include "private/DirtyDefs.hpp"

#include <storm/engine/scenegraph/DirectionalLightNode.hpp>

using namespace storm::engine;

DirectionalLightNode::DirectionalLightNode() : Node() {}

std::uint32_t DirectionalLightNode::dirtyValue() const noexcept {
	return DIRECTIONAL_LIGHT_BITS;
}
