#include "private/DirtyDefs.hpp"

#include <storm/engine/scenegraph/GroupNode.hpp>

using namespace storm::engine;

////////////////////////////////////////
////////////////////////////////////////
GroupNode::GroupNode() : Node() {}

uint32_t GroupNode::dirtyValue() const noexcept { return GROUP_BITS; }
