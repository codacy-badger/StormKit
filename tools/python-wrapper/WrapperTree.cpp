#include "WrapperTree.hpp"

#include <storm/tools/Tree.hpp>

namespace py = pybind11;

using namespace storm::tools;

void initTree(py::module &module) noexcept {
    py::class_<TreeNode>(module, "TreeNode")
      .def(py::init<>())
      .def("invalidate", &TreeNode::invalidate)
      .def_property("parent", &TreeNode::parent, &TreeNode::setParent)
      .def_property("nextSibling", &TreeNode::nextSibling, &TreeNode::setNextSibling)
      .def_property("firstChild", &TreeNode::firstChild, &TreeNode::setFirstChild)
      .def_property("dirtyBits", &TreeNode::dirtyBits, &TreeNode::setDirtyBits)
      .def_property_readonly_static(
        "INVALID_INDEX", [](py::object) { return TreeNode::INVALID_INDEX; });

    py::class_<Tree>(module, "Tree")
      .def(py::init<>())
      .def(
        "insert",
        py::overload_cast<const TreeNode &, TreeNode::Index, TreeNode::Index>(&Tree::insert))
      .def("remove", &Tree::remove)
      .def("markDirty", &Tree::markDirty)
      .def("clearDirties", &Tree::clearDirties)
      .def("genDotFile", py::overload_cast<const std::string &>(&Tree::genDotFile))
      .def("genDotFile", py::overload_cast<const std::string &, int>(&Tree::genDotFile))
      .def_property_readonly("dirties", &Tree::dirties)
      .def_property_readonly("size", &Tree::size)
      .def_property_readonly_static("DEFAULT_PREALLOCATED_TREE_SIZE", [](py::object) {
          return Tree::DEFAULT_PREALLOCATED_TREE_SIZE;
      });
}
