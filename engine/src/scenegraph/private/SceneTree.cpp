#include "SceneTree.hpp"
#include "DirtyDefs.hpp"

#include <storm/log/LogOutput.hpp>

#include <storm/engine/scenegraph/NodeData.hpp>
#include <storm/engine/scenegraph/ViewNode.hpp>
#include <storm/engine/scenegraph/ProgramNode.hpp>
#include <storm/engine/scenegraph/GeoNode.hpp>
#include <storm/engine/scenegraph/GeoInstancedNode.hpp>
#include <storm/engine/scenegraph/MaterialNode.hpp>
//#include <storm/engine/scenegraph/DirectionalLightNode.hpp>

#include <memory>
#include <functional>
#include <set>

using namespace storm::engine;

////////////////////////////////////////
////////////////////////////////////////
SceneTree::SceneTree() = default;

////////////////////////////////////////
////////////////////////////////////////
SceneTree::~SceneTree() = default;

////////////////////////////////////////
////////////////////////////////////////
void SceneTree::onNotified(NodeEvent event, NodePayload &&payload) {
	switch(event) {
		case NodeEvent::ADDED:
			addNode(std::move(payload));
			break;
		case NodeEvent::REMOVED:
			removeNode(std::move(payload));
			break;
		case NodeEvent::UPDATED:
			updateNode(std::move(payload));
			break;
	}
}

////////////////////////////////////////
////////////////////////////////////////
void SceneTree::traverse(DrawableCache &cache) {
	auto m_dirties = m_tree.dirties();
	if(m_dirties.empty()) return;

	auto done = std::set<tools::TreeNode::Index>{};
	auto dirties = TreeNodeList{};
	while(!m_dirties.empty()) {
		auto current_dirty_node = m_dirties.front();
		m_dirties.erase(std::find(std::begin(m_dirties), std::end(m_dirties), current_dirty_node));
		if(std::find(std::begin(dirties), std::end(dirties), current_dirty_node) != std::end(dirties))
			continue;
		if(std::find(std::begin(done), std::end(done), current_dirty_node) != std::end(done))
			continue;

		for(auto parent_node = m_tree[current_dirty_node].parent(); parent_node != tools::TreeNode::INVALID_INDEX; parent_node = m_tree[parent_node].parent()) {
			if(std::find(std::begin(dirties), std::end(dirties), parent_node) != std::end(dirties)) {
				done.emplace(current_dirty_node);
				current_dirty_node = parent_node;
				break;
			}

			if(std::find(std::begin(done), std::end(done), parent_node) != std::end(done)) {
				done.emplace(current_dirty_node);
				current_dirty_node = parent_node;
				break;
			}

			done.emplace(parent_node);

			if(m_tree[parent_node].dirtyBits()) {
				current_dirty_node = parent_node;
				m_dirties.erase(std::find(std::begin(m_dirties), std::end(m_dirties), current_dirty_node));
			}
		}

		if(std::find(std::begin(done), std::end(done), current_dirty_node) != std::end(done))
			continue;

		done.emplace(current_dirty_node);

		dirties.push_back(current_dirty_node);
	}

	auto nodes = std::queue<tools::TreeNode::Index>{};

	for(const auto index : dirties)
		traverseSubTree(index, cache);

	m_tree.clearDirties();
}

////////////////////////////////////////
////////////////////////////////////////
void SceneTree::traverseSubTree(tools::TreeNode::Index index, DrawableCache &cache) {
	/*const auto parent_index = m_tree[index].parent();

	auto stack = std::deque<std::pair<storm::tools::TreeNode::Index, Drawable>>{};
	stack.emplace_back(index, m_cached_drawables[parent_index]);

	while(!stack.empty()) {
			  auto current           = stack.back();
		const auto &current_index    = current.first;
		const auto &node             = m_tree[current_index];
			  auto &drawable         = current.second;
		const auto &cached_drawable  = m_cached_drawables[current_index];
		const auto &graph_node       = m_graph_tree_link[current_index];
		stack.pop_back();

		auto dirty_bits      = node.dirtyBits();
		switch(dirty_bits) {
			case TRANSFORM_BITS | UPDATE_NODE_STATE:
			case TRANSFORM_BITS | AGGREGATE_NODE_STATE:
				drawable.transform *= std::dynamic_pointer_cast<TransformNode>(graph_node)->data().matrix(); // always recompute transform matrix
				drawable.inverted_transform = glm::inverse(drawable.transform);
				dirty_bits = TRANSFORM_BITS;
				break;
			case VIEW_BITS | UPDATE_NODE_STATE: {
				const auto n = std::dynamic_pointer_cast<ViewNode>(graph_node);
				auto data = n->data();
				Renderer::setViewport(RectRef{data.second});
				Renderer::setViewMatrix(data.first);
				break;
			} case PROGRAM_BITS | UPDATE_NODE_STATE:
				drawable.program_id = std::dynamic_pointer_cast<ProgramNode>(graph_node)->data();
				break;
			case PROGRAM_BITS | AGGREGATE_NODE_STATE:
				drawable.program_id = cached_drawable.program_id;
				break;
			case MATERIAL_BITS | UPDATE_NODE_STATE:
				drawable.material = std::dynamic_pointer_cast<MaterialNode>(graph_node)->data();
				break;
			case MATERIAL_BITS | AGGREGATE_NODE_STATE:
				drawable.material = cached_drawable.material;
				break;
			case GEOMETRY_BITS | UPDATE_NODE_STATE: {
				const auto n = std::dynamic_pointer_cast<GeoNode>(graph_node);
				const auto &geometry = n->data();

				drawable.vertices = geometry.vertices;
				drawable.indices = geometry.indices;
				drawable.indexed = !drawable.indices.empty();

				drawable.index = current_index;
				cache.addOrUpdateDrawable(drawable);
				break;
			} case GEOMETRY_BITS | AGGREGATE_NODE_STATE: {
				auto _drawable = cached_drawable;
				_drawable.transform  = drawable.transform;
				_drawable.material   = drawable.material;
				_drawable.program_id = drawable.program_id;
				if(drawable.instanced) {
					_drawable.instanced      = drawable.instanced;
					_drawable.instances      = drawable.instances;
				}

				std::swap(_drawable, drawable);
				cache.addOrUpdateDrawable(drawable);
				break;
			} case GEOMETRY_INSTANCED_BITS | UPDATE_NODE_STATE: {
				const auto n = std::dynamic_pointer_cast<GeoInstancedNode>(graph_node);
				const auto &instances = n->data();

				auto instances_ = Drawable::Instances();
				instances_.count = instances.size();
				for(const auto &instance : instances) {
					const auto matrix = instance.second->transform.matrix();
					instances_.diffuse.push_back(instance.second->material.diffuse());
					instances_.transform.push_back(matrix);
					instances_.inverted_transform.emplace_back(glm::inverse(matrix));
				}

				drawable.instances = std::move(instances_);
				drawable.instanced = true;

				dirty_bits = GEOMETRY_INSTANCED_BITS;
				break;
			} case SCENE_BITS | UPDATE_NODE_STATE:
				Renderer::setProjectionMatrix(std::dynamic_pointer_cast<SceneNode>(graph_node)->data());
				break;
			case DIRECTIONAL_LIGHT_BITS | UPDATE_NODE_STATE:
				//Renderer::setDirectionalLight(std::dynamic_pointer_cast<DirectionalLightNode>(graph_node)->data());
				break;
		}

		m_cached_drawables[current_index] = drawable;

		dirty_bits &= 0x0F;
		for(auto child = node.firstChild(); child != tools::TreeNode::INVALID_INDEX; child = m_tree[child].nextSibling()) {
			if(m_tree[child].dirtyBits() == 0 && (dirty_bits == TRANSFORM_BITS || dirty_bits == GEOMETRY_INSTANCED_BITS  || dirty_bits == GROUP_BITS))
				m_tree.markDirty(child,  m_graph_tree_link[child]->dirtyValue() | AGGREGATE_NODE_STATE);
			stack.emplace_back(child, drawable);
		}
	}*/
}

////////////////////////////////////////
////////////////////////////////////////
void SceneTree::addNode(NodePayload &&payload) {
	const auto &parents = payload.first->parents();

	if(!parents.size()) {
		auto node = tools::TreeNode{};
		auto node_index = m_tree.insert(node, tools::TreeNode::INVALID_INDEX, tools::TreeNode::INVALID_INDEX);
		m_tree.markDirty(node_index, payload.second);
		m_graph_tree_link.emplace(node_index, std::move(payload.first));
	} else {
		for(const auto &parent : parents) {
			auto parent_tree_indices = TreeNodeList();
			std::for_each(std::begin(m_graph_tree_link), std::end(m_graph_tree_link), [&parent, &parent_tree_indices](auto data) {
				if(data.second == parent)
					parent_tree_indices.push_back(data.first);
			});

			for(const auto parent_tree_index : parent_tree_indices) {
				auto node = tools::TreeNode{};
				auto node_index = m_tree.insert(node, parent_tree_index, tools::TreeNode::INVALID_INDEX);
				m_tree.markDirty(node_index, payload.second);
				m_graph_tree_link.emplace(node_index, std::move(payload.first));
			}
		}
	}
}

////////////////////////////////////////
////////////////////////////////////////
void SceneTree::removeNode(NodePayload &&payload) {

}

////////////////////////////////////////
////////////////////////////////////////
void SceneTree::updateNode(NodePayload &&payload) {
	auto tree_nodes = TreeNodeList();
	std::for_each(std::begin(m_graph_tree_link), std::end(m_graph_tree_link), [&payload, &tree_nodes](auto data) {
		if(data.second == payload.first)
			tree_nodes.push_back(data.first);
	});

	for(auto &node_index : tree_nodes)
		m_tree.markDirty(node_index, payload.second);
}
