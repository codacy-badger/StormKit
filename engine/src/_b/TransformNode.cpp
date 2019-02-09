#include <storm/engine/scenegraph/TransformNode.hpp>
#include "private/DirtyDefs.hpp"

using namespace storm::engine;

std::uint32_t TransformNode::dirtyValue() const noexcept {
	return TRANSFORM_BITS;
}
