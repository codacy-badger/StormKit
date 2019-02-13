#include "private/DirtyDefs.hpp"

#include <storm/engine/scenegraph/MaterialNode.hpp>

using namespace storm::engine;

MaterialNode::MaterialNode() : Node() {}

std::uint32_t MaterialNode::dirtyValue() const noexcept {
	return MATERIAL_BITS;
}
