#include <storm/tools/Tree.hpp>

#include <functional>
#include <queue>
#include <fstream>

using namespace storm::tools;

////////////////////////////////////////
////////////////////////////////////////
Tree::Tree() : m_first_free_index(0) {
	m_tree.resize(DEFAULT_PREALLOCATED_TREE_SIZE);

	for(auto i = TreeNode::Index (0); i < m_tree.size() - 1; ++i)
		m_tree[i].setNextSibling(i + 1);
}

////////////////////////////////////////
////////////////////////////////////////
TreeNode::Index Tree::getFreeNode() {
	if(m_tree[m_first_free_index].nextSibling() == TreeNode::INVALID_INDEX ) {
		auto size     = float(m_tree.size());
		auto first_new = TreeNode::Index(size);

		m_tree.resize(static_cast<std::size_t>(size * 1.5f));
		auto new_size = m_tree.size();

		  // generate a new chain of free objects, with the last one pointing to ~0
		m_tree[m_first_free_index].setNextSibling(first_new);
		for(auto i = first_new; i < new_size - 1; ++i)
			m_tree[i].setNextSibling(i + 1);
	}

	auto index = m_first_free_index;
	m_first_free_index = m_tree[m_first_free_index].nextSibling();
	return index;
}

////////////////////////////////////////
////////////////////////////////////////
TreeNode::Index Tree::insert(const TreeNode &node, TreeNode::Index parent_index, TreeNode::Index previous_sibling) {
	const auto index = getFreeNode();

	auto &_node = m_tree[index];
	_node = node;

	_node.setParent(parent_index);

	//check if parent is real node
	if(parent_index != TreeNode::INVALID_INDEX) {
		auto &parent_node = *(std::begin(m_tree) + parent_index);

		//new node is first child
		if(parent_node.firstChild() == TreeNode::INVALID_INDEX)
			parent_node.setFirstChild(index);
		else if(previous_sibling == TreeNode::INVALID_INDEX) { //insert a beginning of childs
			_node.setNextSibling(parent_node.firstChild());
			parent_node.setFirstChild(index);
		} else { // insert at the end
			auto prev_sibling_node = m_tree[previous_sibling];
			_node.setNextSibling(prev_sibling_node.nextSibling());
			prev_sibling_node.setNextSibling(index);
		}
	}

	return index;
}

////////////////////////////////////////
////////////////////////////////////////
TreeNode::Index Tree::insert(TreeNode &&node, TreeNode::Index parent_index, TreeNode::Index previous_sibling) {
	const auto index = getFreeNode();

	auto &_node = m_tree[index];
	_node = std::move(node);

	_node.setParent(parent_index);

	//check if parent is real node
	if(parent_index != TreeNode::INVALID_INDEX) {
		auto &parent_node = *(std::begin(m_tree) + parent_index);

		//new node is first child
		if(parent_node.firstChild() == TreeNode::INVALID_INDEX)
			parent_node.setFirstChild(index);
		else if(previous_sibling == TreeNode::INVALID_INDEX) { //insert a beginning of childs
			_node.setNextSibling(parent_node.firstChild());
			parent_node.setFirstChild(index);
		} else { // insert at the end
			auto prev_sibling_node = m_tree[previous_sibling];
			_node.setNextSibling(prev_sibling_node.nextSibling());
			prev_sibling_node.setNextSibling(index);
		}
	}

	//markDirty(index, 0b10000);

	return index;
}

////////////////////////////////////////
////////////////////////////////////////
void Tree::remove(TreeNode::Index index) {
	auto &node = m_tree[index];

	if(node.parent() != TreeNode::INVALID_INDEX) {
		auto &parent = m_tree[node.parent()];

		//Remove sibling
		auto current_index = parent.firstChild();
		while(current_index != TreeNode::INVALID_INDEX) {
			auto &current_node = m_tree[current_index];

			if(current_node.nextSibling() == index) {
				current_node.setNextSibling(node.nextSibling());
				break;
			}
			current_index = current_node.nextSibling();
		}

		//remove parent
		if(parent.firstChild() == index)
			parent.setFirstChild(node.nextSibling());

		node.setParent(TreeNode::INVALID_INDEX);
	}

	auto last_index = TreeNode::INVALID_INDEX;
	std::deque<TreeNode::Index> queue;
	queue.emplace_back(index);
	while(!queue.empty()) {
		auto current_index = queue.front();
		auto &current_node  = m_tree[current_index];
		queue.pop_front();

		auto child_index = current_node.firstChild();
		while(child_index != TreeNode::INVALID_INDEX) {
			queue.emplace_back(child_index);
			child_index = m_tree[child_index].nextSibling();
		}

		node.invalidate();

		if(last_index != TreeNode::INVALID_INDEX)
			m_tree[last_index].setNextSibling(current_index);

		last_index = current_index;
	}

	m_tree[last_index].setNextSibling(m_first_free_index);
	m_first_free_index = index;
}

////////////////////////////////////////
////////////////////////////////////////
void Tree::markDirty(TreeNode::Index index, TreeNode::DirtyBits bits) {
	auto &node = m_tree[index];
	if(!node.dirtyBits()) {
		m_dirties.emplace_back(index);
		node.setDirtyBits(bits);
		return;
	}
	node.setDirtyBits(node.dirtyBits() | bits);
}

std::string colorFromNode(const std::string &name) {
	if(name == "SceneNode")
		return ".7 .3 1.0";
	else if(name =="ProgramNode")
		return "0.3 0.2 0.5";
	else if(name =="GroupNode")
		return "0.9 0.1 0.3";
	else if(name =="MaterialNode")
		return "0.4 0.1 0.4";
	else if(name =="GeoNode")
		return "0.1 0.2 0.3";
	else if(name =="TransformNode")
		return "0.1 0.2 0.7";
	else if(name =="GeoInstancedNode")
		return "0.5 0.1 0.4";

	return "1.0 1.0 1.0";
}

void Tree::genDotFile(const std::string &filename) {
	std::fstream stream(filename, std::ios::out);

	stream << "digraph G { \n"
			  "node [shape=box];\n";

	for(auto i = 0;i < m_first_free_index; ++i) {
		const auto name = operator[](i).name();
		const auto dirty = bool(operator[](i).dirtyBits());
		stream << "    Node" << i << "[label=\"id: " << i << " type: " << name << " dirty: " << std::boolalpha << dirty << "\", style=filled,color=\"" << colorFromNode(name) << "\"];\n";
	}

	for(auto i = 0;i < m_first_free_index; ++i) {
		if(operator[](i).firstChild() == TreeNode::INVALID_INDEX)
			continue;

		for(auto current = operator[](i).firstChild(); current != TreeNode::INVALID_INDEX; current =  operator[](current).nextSibling()) {
			stream << "    Node" << i << " -> Node" << current  << ";\n";
		}
	}

	stream << "}" << std::flush;

	stream.close();
}

void Tree::genDotFile(const std::string &filename, int highlight) {
	std::fstream stream(filename, std::ios::out);

	stream << "digraph G { \n"
			  "node [shape=box];\n";

	for(auto i = 0;i < m_first_free_index; ++i) {
		const auto name = operator[](i).name();
		const auto dirty = bool(operator[](i).dirtyBits());
		if(i != highlight)
			stream << "    Node" << i << "[label=\"id: " << i << " type: " << name << " dirty: " << std::boolalpha << dirty << "\", style=filled,color=\"" << colorFromNode(name) << "\"];\n";
		else
			stream << "    Node" << i << "[shape=polygon,sides=5,peripheries=3, label=\"id: " << i << " type: " << name << " dirty: " << std::boolalpha << dirty << "\", style=filled,color=\"" << colorFromNode(name) << "\"];\n";
	}

	for(auto i = 0;i < m_first_free_index; ++i) {
		if(operator[](i).firstChild() == TreeNode::INVALID_INDEX)
			continue;

		for(auto current = operator[](i).firstChild(); current != TreeNode::INVALID_INDEX; current =  operator[](current).nextSibling()) {
			stream << "    Node" << i << " -> Node" << current  << ";\n";
		}
	}

	stream << "}" << std::flush;

	stream.close();
}
