#include "Wrapper.hpp"

#include "WrapperState.hpp"
#include "WrapperTree.hpp"

#include <storm/core/Configure.hpp>
#include <storm/tools/Tree.hpp>
#include <storm/tools/TreeNode.hpp>

//#include <Python.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

using namespace storm::tools;

PYBIND11_MODULE(tools, tools_module) {
	tools_module.doc() = "Stormkit C++ tools module python binding";

	initState(tools_module);
	initTree(tools_module);
	tools_module.attr("__version__") = STORM_VERSION;
}
