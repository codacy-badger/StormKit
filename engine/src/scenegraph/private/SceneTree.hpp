#pragma once

#include <storm/tools/Subject.hpp>
#include <storm/tools/Tree.hpp>
#include <storm/tools/TreeNode.hpp>

#include <storm/engine/graphics/Geometry.hpp>
#include <storm/engine/graphics/Material.hpp>
#include <storm/engine/graphics/Transform.hpp>

#include <storm/engine/scenegraph/SceneNode.hpp>
#include <storm/engine/scenegraph/TransformNode.hpp>

namespace storm::engine {
	class Mesh {
		std::string                       program_name;
		std::reference_wrapper<Geometry>  geometry;
		std::reference_wrapper<Material>  material;
		Transform                         transform;
	};

	class DrawableCache;
	class SceneTree : public NodeObserver {
		public:
			SUR_Object(SceneTree)

			using MeshList     = std::vector<Mesh>;
			using TreeNodeList = std::vector<tools::TreeNode::Index>;

			explicit SceneTree();
			~SceneTree() override;

			void onNotified(NodeEvent event, NodePayload &&payload) override;

			void traverse(DrawableCache &cache);
			const MeshList &shapes() const noexcept { return m_shapes; }
		private:
			void traverseSubTree(tools::TreeNode::Index index, DrawableCache &cache);
			void addNode(NodePayload &&payload);
			void removeNode(NodePayload &&payload);
			void updateNode(NodePayload &&payload);

			NodeArray findFirstUpdatedNodes(const SceneNode::Ptr &node);

			inline bool isTransformNode(const Node::Ptr &node) const noexcept { return dynamic_cast<TransformNode*>(node.get()); }

			tools::Tree m_tree;

			MeshList                                               m_shapes;
			std::unordered_map<tools::TreeNode::Index, std::size_t> m_shape_link;
			std::unordered_map<tools::TreeNode::Index, Node::Ptr>   m_graph_tree_link;
			//std::unordered_map<tools::TreeNode::Index, Drawable>    m_cached_drawables;
	};
}
