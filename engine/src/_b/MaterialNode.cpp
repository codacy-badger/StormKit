#include <storm/engine/scenegraph/MaterialNode.hpp>
#include "private/DirtyDefs.hpp"

using namespace storm::engine;

MaterialNode::MaterialNode()
	: Node() {

}

std::uint32_t MaterialNode::dirtyValue() const noexcept {
	return MATERIAL_BITS;
}
