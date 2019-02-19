#include <functional>
#include <memory>
#include <set>
#include <storm/engine/render/Device.hpp>
#include <storm/engine/scenegraph/CameraNode.hpp>
#include <storm/engine/scenegraph/DirtyDefs.hpp>
#include <storm/engine/scenegraph/GeometryNode.hpp>
#include <storm/engine/scenegraph/NodeData.hpp>
#include <storm/engine/scenegraph/SceneTree.hpp>
#include <storm/log/LogOutput.hpp>

using namespace storm::engine;

////////////////////////////////////////
////////////////////////////////////////
SceneTree::SceneTree(const Device &device) : m_device {device} {}

////////////////////////////////////////
////////////////////////////////////////
SceneTree::~SceneTree() = default;

////////////////////////////////////////
////////////////////////////////////////
void SceneTree::onNotified(NodeEvent event, NodePayload &&payload) {
	switch (event) {
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
void SceneTree::exportSceneTree(const _std::filesystem::path &filepath,
    std::function<std::string_view(std::string_view)> colorize_node) const
    noexcept {
	m_tree.genDotFile(filepath.string(), colorize_node);
}

////////////////////////////////////////
////////////////////////////////////////
void SceneTree::traverse(MeshList &mesh_list) {
	auto m_dirties = m_tree.dirties();
	if (m_dirties.empty())
		return;

	auto done    = std::set<tools::TreeNode::Index> {};
	auto dirties = TreeNodeList {};
	while (!m_dirties.empty()) {
		auto current_dirty_node = m_dirties.front();
		m_dirties.erase(std::find(
		    std::begin(m_dirties), std::end(m_dirties), current_dirty_node));
		if (std::find(
		        std::begin(dirties), std::end(dirties), current_dirty_node)
		    != std::end(dirties))
			continue;
		if (std::find(std::begin(done), std::end(done), current_dirty_node)
		    != std::end(done))
			continue;

		for (auto parent_node = m_tree[current_dirty_node].parent();
		     parent_node != tools::TreeNode::INVALID_INDEX;
		     parent_node = m_tree[parent_node].parent()) {
			if (std::find(std::begin(dirties), std::end(dirties), parent_node)
			    != std::end(dirties)) {
				done.emplace(current_dirty_node);
				current_dirty_node = parent_node;
				break;
			}

			if (std::find(std::begin(done), std::end(done), parent_node)
			    != std::end(done)) {
				done.emplace(current_dirty_node);
				current_dirty_node = parent_node;
				break;
			}

			done.emplace(parent_node);

			if (m_tree[parent_node].dirtyBits()) {
				current_dirty_node = parent_node;
				m_dirties.erase(std::find(std::begin(m_dirties),
				    std::end(m_dirties), current_dirty_node));
			}
		}

		if (std::find(std::begin(done), std::end(done), current_dirty_node)
		    != std::end(done))
			continue;

		done.emplace(current_dirty_node);

		dirties.push_back(current_dirty_node);
	}

	auto nodes = std::queue<tools::TreeNode::Index> {};

	for (const auto index : dirties)
		traverseSubTree(index, mesh_list);

	m_tree.clearDirties();
}

////////////////////////////////////////
////////////////////////////////////////
void SceneTree::traverseSubTree(
    tools::TreeNode::Index index, MeshList &mesh_list) {
	auto stack = std::stack<storm::tools::TreeNode::Index> {};
	stack.emplace(index);

	while (!stack.empty()) {
		auto current_tree_node_index = stack.top();
		stack.pop();

		auto &      current_state     = m_states[current_tree_node_index];
		const auto &current_tree_node = m_tree[current_tree_node_index];
		const auto &current_graph_node
		    = m_graph_tree_link.at(current_tree_node_index).get();

		const auto  parent_index = m_tree[current_tree_node_index].parent();
		const auto &parent_state = m_states[parent_index];

		auto dirty_bits = current_tree_node.dirtyBits();
		switch (dirty_bits) {
		case CAMERA_BITS | CREATE_NODE_STATE:
		case CAMERA_BITS | UPDATE_NODE_STATE: {
			current_state = parent_state;

			const auto &data_node
			    = static_cast<const CameraNode &>(current_graph_node);
			const auto &data = data_node.data();

			current_state.projection = data.projection;
			current_state.view       = data.view;
			break;
		}
		case TRANSFORM_BITS | CREATE_NODE_STATE:
		case TRANSFORM_BITS | UPDATE_NODE_STATE: {
			current_state = parent_state;

			const auto &data_node
			    = static_cast<const TransformNode &>(current_graph_node);
			const auto &data = data_node.data();

			current_state.model
			    *= data.matrix(); // always recompute transform matrix
			current_state.inverted_model = glm::inverse(current_state.model);
			break;
		}
		case GEOMETRY_BITS | CREATE_NODE_STATE: {
			current_state = parent_state;

			const auto &data_node
			    = static_cast<const GeometryNode &>(current_graph_node);
			const auto &data = data_node.data();

			auto  pair = mesh_list.addMesh();
			auto  id   = pair.first;
			auto &mesh = pair.second;

			const auto vertex_buffer_desc = HardwareBuffer::Description {
				data.vertices.size,
				data.vertices.alignement,
                BufferUsageFlag::VERTEX
			};

			mesh.get().vertex_buffer = m_device.get().createHardwareBufferPtr(
				std::move(vertex_buffer_desc));
			mesh.get().vertex_state = data.vertex_state;

			std::visit(
			    [&](const auto &vertices) {
					mesh.get().vertex_buffer->setData(vertices);
				    mesh.get().vertex_count = std::size(vertices);
			    },
			    data.vertices.array);

			if (data.indices.size) {
				const auto index_buffer_desc = HardwareBuffer::Description {
					data.indices.size,
					data.indices.alignement,
                    BufferUsageFlag::INDEX
				};

				mesh.get().index_buffer = m_device.get().createHardwareBufferPtr(
					std::move(index_buffer_desc));

				std::visit(core::overload {
				               [&](const IndexArray &indices) {
								   mesh.get().index_buffer->setData(indices);
					               mesh.get().index_count = std::size(indices);
				               },
				               [&](const LargeIndexArray &indices) {
								   mesh.get().index_buffer->setData(indices);
					               mesh.get().large_indices = true;
					               mesh.get().index_count = std::size(indices);
				               }},
				    data.indices.array);
			}
			mesh.get().projection = current_state.projection;
			mesh.get().view       = current_state.view;
			mesh.get().model      = current_state.model;
			current_state.mesh_id = id;
			break;
		}
		case GEOMETRY_BITS | UPDATE_NODE_STATE: {
			auto mesh_id = current_state.mesh_id;

			current_state         = parent_state;
			current_state.mesh_id = mesh_id;

			auto &mesh      = mesh_list.mesh(current_state.mesh_id);
			mesh.projection = current_state.projection;
			mesh.view       = current_state.view;
			mesh.model      = current_state.model;
		}
		}

		for (auto child = current_tree_node.firstChild();
		     child != tools::TreeNode::INVALID_INDEX;
		     child = m_tree[child].nextSibling()) {
			if (m_tree[child].dirtyBits() == 0)
				m_tree.markDirty(
				    child, m_graph_tree_link.at(child).get().dirtyValue()
				               | UPDATE_NODE_STATE);

			stack.emplace(child);
		}
	}

	/*	dirty_bits &= 0x0F;
	    for(auto child = node.firstChild(); child !=
	tools::TreeNode::INVALID_INDEX; child = m_tree[child].nextSibling()) {
	        if(m_tree[child].dirtyBits() == 0 && dirty_bits == TRANSFORM_BITS)
	            m_tree.markDirty(child,  m_graph_tree_link[child]->dirtyValue()
	| AGGREGATE_NODE_STATE); stack.emplace_back(child, drawable);
	    }
	}*/
}

////////////////////////////////////////
////////////////////////////////////////
void SceneTree::addNode(NodePayload &&payload) {
	const auto &parents = payload.node.get().parents();

	if (!parents.size()) {
		auto node = tools::TreeNode {};
		node.setName(payload.node.get().name());
		auto node_index = m_tree.insert(node, tools::TreeNode::INVALID_INDEX,
		    tools::TreeNode::INVALID_INDEX);

		m_tree.markDirty(
		    node_index, payload.node.get().dirtyValue() | CREATE_NODE_STATE);

		m_graph_tree_link.emplace(node_index, std::move(payload.node));
	} else {
		for (const auto &parent : parents) {
			auto parent_tree_indices = std::vector<tools::TreeNode::Index> {};
			std::for_each(std::begin(m_graph_tree_link),
			    std::end(m_graph_tree_link),
			    [&parent, &parent_tree_indices](const auto &data) {
				    if (data.second.get().id() == parent.get().id())
					    parent_tree_indices.emplace_back(data.first);
			    });

			for (const auto parent_tree_index : parent_tree_indices) {
				auto node = tools::TreeNode {};
				node.setName(payload.node.get().name());
				auto node_index = m_tree.insert(
				    node, parent_tree_index, tools::TreeNode::INVALID_INDEX);
				m_tree.markDirty(node_index,
				    payload.node.get().dirtyValue() | CREATE_NODE_STATE);
				m_graph_tree_link.emplace(node_index, std::move(payload.node));
			}
		}
	}
}

////////////////////////////////////////
////////////////////////////////////////
void SceneTree::removeNode(NodePayload &&payload) {}

////////////////////////////////////////
////////////////////////////////////////
void SceneTree::updateNode(NodePayload &&payload) {
	auto tree_nodes = std::vector<tools::TreeNode::Index>();
	std::for_each(std::begin(m_graph_tree_link), std::end(m_graph_tree_link),
	    [&payload, &tree_nodes](const auto &data) {
		    if (&data.second.get() == &payload.node.get())
			    tree_nodes.push_back(data.first);
	    });

	for (auto &node_index : tree_nodes)
		if (!m_tree[node_index].dirtyBits())
			m_tree.markDirty(node_index,
			    payload.node.get().dirtyValue() | UPDATE_NODE_STATE);
}
