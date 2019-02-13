#include "private/DirtyDefs.hpp"

#include <storm/engine/scenegraph/ProgramNode.hpp>

using namespace storm::engine;

////////////////////////////////////////
////////////////////////////////////////
uint32_t ProgramNode::dirtyValue() const noexcept { return PROGRAM_BITS; }
