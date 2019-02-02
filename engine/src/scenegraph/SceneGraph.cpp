#include <storm/engine/scenegraph/SceneGraph.hpp>

#include "private/SceneTree.hpp"

using namespace storm::engine;

////////////////////////////////////////
////////////////////////////////////////
SceneGraph::SceneGraph() : m_root(nullptr) {
	m_root = SceneNode::makeShared();
	m_tree = SceneTree::makeShared();

	m_root->notify(NodeEvent::ADDED);
	m_root->setObserver(m_tree);
}

////////////////////////////////////////
////////////////////////////////////////
SceneGraph::~SceneGraph() = default;

////////////////////////////////////////
////////////////////////////////////////
/*SceneTree &SceneGraph::updateCache(DrawableCache &cache) {
	m_tree->traverse(cache);

	return *m_tree;
}*/



