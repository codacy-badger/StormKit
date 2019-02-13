#include "private/DirtyDefs.hpp"

#include <storm/engine/scenegraph/ViewNode.hpp>

using namespace storm::engine;

////////////////////////////////////////
////////////////////////////////////////
ViewNode::ViewNode() : Node(), m_data {std::make_pair<>(mat4(1.f), vec4(0.f))} {
	m_view = View::makeShared();
}

////////////////////////////////////////
////////////////////////////////////////
uint32_t ViewNode::dirtyValue() const noexcept { return VIEW_BITS; }

////////////////////////////////////////
////////////////////////////////////////
void ViewNode::commit() noexcept {
	m_data.first  = m_view->matrix();
	m_data.second = m_view->viewport();

	notify(NodeEvent::UPDATED);
}
