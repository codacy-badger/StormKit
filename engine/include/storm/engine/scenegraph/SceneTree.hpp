#pragma once

#include <vector>

#include <storm/tools/Subject.hpp>
#include <storm/tools/Tree.hpp>
#include <storm/tools/TreeNode.hpp>

#include <storm/engine/render/ForwardDeclarations.hpp>

#include <storm/engine/graphics/MeshList.hpp>

#include <storm/engine/scenegraph/SceneNode.hpp>
#include <storm/engine/scenegraph/TransformNode.hpp>

namespace storm::engine {
	class SceneTree : public NodeObserver {
		public:
			SUR_Object(SceneTree)

			using TreeNodeList = std::vector<tools::TreeNode::Index>;

			explicit SceneTree(const Device &device);
			~SceneTree() override;

			void onNotified(NodeEvent event, NodePayload &&payload) override;

			void exportSceneTree(const _std::filesystem::path &filepath, std::function<std::string_view(std::string_view)> colorize_node) const noexcept;

			void traverse(MeshList &mesh_list);
		private:
			struct State {
				mat4 projection;
				mat4 view;

				mat4 model;
				mat4 inverted_model;

				std::int64_t mesh_id = -1;
			};

			void traverseSubTree(tools::TreeNode::Index index, MeshList &mesh_list);
			void addNode(NodePayload &&payload);
			void removeNode(NodePayload &&payload);
			void updateNode(NodePayload &&payload);

			SceneNodeArray findFirstUpdatedNodes(const SceneNode &node);

			//inline bool isTransformNode(const SceneNode &node) const noexcept { return static_cast<TransformNode&>(node.get()); }

			std::reference_wrapper<const Device> m_device;

			tools::Tree m_tree;

			std::unordered_map<tools::TreeNode::Index, SceneNode::RefW> m_graph_tree_link;
			std::unordered_map<tools::TreeNode::Index, State> m_states;


	};
}

#include "SceneTree.inl"
