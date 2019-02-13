#include "private/DirtyDefs.hpp"

#include <algorithm>
#include <storm/engine/scenegraph/Node.hpp>
#include <storm/engine/scenegraph/SceneGraph.hpp>

using namespace storm::engine;

////////////////////////////////////////
////////////////////////////////////////
Node::Node() : NodeSubject(), m_graph(nullptr) {}

////////////////////////////////////////
////////////////////////////////////////
Node::~Node() = default;

////////////////////////////////////////
////////////////////////////////////////
void Node::addChild(const Node::Ptr &child) {
	child->m_parents.push_back(shared_from_this());
	m_childs.emplace_back(child);

	child->notify(NodeEvent::ADDED);

	if (observer())
		child->setObserver(observer());
}

////////////////////////////////////////
////////////////////////////////////////
void Node::removeChild(const Node::Ptr &child) {
	auto it = std::find(std::begin(m_childs), std::end(m_childs), child);

	if (it == std::end(m_childs))
		return;

	auto parent_it = std::find(std::begin(child->m_parents),
	    std::end(child->m_parents), shared_from_this());
	child->m_parents.erase(parent_it);

	m_childs.erase(it);

	child->notify(NodeEvent::REMOVED);
}

////////////////////////////////////////
////////////////////////////////////////
void Node::setObserver(NodeObserver::Ptr &&observer) {
	registerObserver(std::forward<NodeObserver::Ptr>(observer));

	for (auto &i : m_childs) {
		i->setObserver(this->observer());
	}
}

////////////////////////////////////////
////////////////////////////////////////
void Node::notify(NodeEvent event) noexcept {
	Subject::notify(std::move(event),
	    std::make_pair<>(shared_from_this(), dirtyValue() | UPDATE_NODE_STATE));
}
