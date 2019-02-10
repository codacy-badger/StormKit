#include <storm/engine/scenegraph/GeoNode.hpp>
#include "private/DirtyDefs.hpp"

using namespace storm::engine;

////////////////////////////////////////
////////////////////////////////////////
std::uint32_t GeoNode::dirtyValue() const noexcept {
	return GEOMETRY_BITS;
}
