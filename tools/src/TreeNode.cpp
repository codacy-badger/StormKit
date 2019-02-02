#include <storm/tools/TreeNode.hpp>


using namespace storm::tools;

constexpr TreeNode::Index TreeNode::INVALID_INDEX;

/////////////////////////////////////
/////////////////////////////////////
void TreeNode::invalidate() {
	m_parent       = INVALID_INDEX;
	m_next_sibling = INVALID_INDEX;
	m_first_child  = INVALID_INDEX;
	m_dirty_bits   = 0;
	m_name = "";
}
