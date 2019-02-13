#include "private/DirtyDefs.hpp"

#include <storm/engine/scenegraph/GeoInstancedNode.hpp>

using namespace storm::engine;

////////////////////////////////////////
////////////////////////////////////////
GeoInstancedNode::GeoInstancedNode() : Node(), m_next_instance_id(0) {}

////////////////////////////////////////
////////////////////////////////////////
void GeoInstancedNode::clear() { m_instances.clear(); }

////////////////////////////////////////
////////////////////////////////////////
void GeoInstancedNode::removeInstance(std::uint32_t id) {
	auto it = m_instances.find(id);

	if (it == std::end(m_instances))
		return;

	m_instances.erase(it);
}

////////////////////////////////////////
////////////////////////////////////////
std::uint32_t GeoInstancedNode::dirtyValue() const noexcept {
	return GEOMETRY_INSTANCED_BITS;
}
