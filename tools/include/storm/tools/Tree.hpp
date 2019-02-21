#pragma once
#include "TreeNode.hpp"

#include <vector>

namespace storm::tools {
    class STORM_PUBLIC Tree final {
    public:
        constexpr static const std::size_t DEFAULT_PREALLOCATED_TREE_SIZE = 1000;

        explicit Tree();
        ~Tree() = default;

        Tree(const Tree &tree) = default;
        Tree(Tree &&tree)      = default;
        Tree &operator=(const Tree &tree) = default;
        Tree &operator=(Tree &&tree) = default;

        TreeNode::Index getFreeNode();

        TreeNode::Index insert(
          const TreeNode &node,
          TreeNode::Index parent_index,
          TreeNode::Index previous_sibling);
        TreeNode::Index
        insert(TreeNode &&node, TreeNode::Index parent_index, TreeNode::Index previous_sibling);
        void remove(TreeNode::Index index);

        void markDirty(TreeNode::Index index, TreeNode::DirtyBits bits);

        inline const TreeNode &operator[](TreeNode::Index index) const { return m_tree[ index ]; }
        inline TreeNode &operator[](TreeNode::Index index) { return m_tree[ index ]; }

        inline std::size_t size() const { return m_tree.size(); }

        inline std::vector<TreeNode>::iterator begin() { return std::begin(m_tree); }
        inline const std::vector<TreeNode>::const_iterator begin() const {
            return std::cbegin(m_tree);
        }
        inline const std::vector<TreeNode>::const_iterator cbegin() const {
            return std::cbegin(m_tree);
        }

        inline std::vector<TreeNode>::iterator end() { return std::end(m_tree); }
        inline const std::vector<TreeNode>::const_iterator end() const { return std::cend(m_tree); }
        inline const std::vector<TreeNode>::const_iterator cend() const {
            return std::cend(m_tree);
        }

        inline void clearDirties() {
            for(auto i : m_dirties) { m_tree[ i ].setDirtyBits(0); }
            m_dirties.clear();
        }
        inline const std::vector<TreeNode::Index> &dirties() const noexcept { return m_dirties; }

        void genDotFile(
          const std::string &filename,
          std::function<std::string_view(std::string_view)> colorize_node) const;
        void genDotFile(
          const std::string &filename,
          int highlight,
          std::function<std::string_view(std::string_view)> colorize_node) const;

    private:
        TreeNode::Index m_first_free_index;
        std::vector<TreeNode> m_tree;
        std::vector<TreeNode::Index> m_dirties;
    };
} // namespace storm::tools
