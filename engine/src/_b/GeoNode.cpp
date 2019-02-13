#include "private/DirtyDefs.hpp"

#include <storm/engine/scenegraph/GeoNode.hpp>

using namespace storm::engine;

////////////////////////////////////////
////////////////////////////////////////
std::uint32_t GeoNode::dirtyValue() const noexcept { return GEOMETRY_BITS; }
