// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include "DefaultRenderTaskData.hpp"

#include <fstream>
#include <storm/engine/render/Surface.hpp>
#include <storm/engine/scenegraph/CameraNode.hpp>
#include <storm/engine/scenegraph/GeometryNode.hpp>
#include <storm/engine/scenegraph/Scene.hpp>
#include <storm/engine/scenegraph/TransformNode.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
constexpr std::string_view colorFromNode(std::string_view name) {
	if (name == "CameraNode")
		return ".7 .3 1.0";
	else if (name == "ProgramNode")
		return "0.3 0.2 0.5";
	else if (name == "GroupNode")
		return "0.9 0.1 0.3";
	else if (name == "MaterialNode")
		return "0.4 0.1 0.4";
	else if (name == "GeometryNode")
		return "0.1 0.2 0.3";
	else if (name == "TransformNode")
		return "0.1 0.2 0.7";

	return "1.0 1.0 1.0";
}

/////////////////////////////////////
/////////////////////////////////////
template <typename T>
constexpr bool is(const SceneNode *node) {
	return dynamic_cast<const T *>(node) != nullptr;
}

/////////////////////////////////////
/////////////////////////////////////
Scene::Scene(const Device &device)
    : m_device {device}, m_tree {m_device}, m_root {*this} {
	m_root.notify(NodeEvent::ADDED);
	m_root.setObserver(&m_tree);
}

/////////////////////////////////////
/////////////////////////////////////
Scene::~Scene() = default;

/////////////////////////////////////
/////////////////////////////////////
Scene::Scene(Scene &&) = default;

/////////////////////////////////////
/////////////////////////////////////
Scene &Scene::operator=(Scene &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void Scene::update() {
	m_tree.traverse(m_cache);

	m_device.get().waitIdle();
}

/////////////////////////////////////
/////////////////////////////////////
void Scene::exportSceneGraph(const _std::filesystem::path &filepath) const {
	using namespace std::literals;

	auto stream = std::ofstream {filepath.string(), std::ios::out};

	stream << "digraph G { \n"
	       << "    rankdir = LR\n"
	       << "    bgcolor = black\n\n"
	       << "    node [shape=rectangle, fontname=\"helvetica\", "
	          "fontsize=12];\n\n";

	auto node_stack = std::stack<const SceneNode *> {};
	auto node_done  = std::vector<const SceneNode *> {};
	node_stack.emplace(&m_root);

	while (!node_stack.empty()) {
		auto node = node_stack.top();
		node_stack.pop();

		auto name = "Unknown Node"sv;

		if (is<CameraNode>(node))
			name = nameOfNode<CameraNode>();
		else if (is<GeometryNode>(node))
			name = nameOfNode<GeometryNode>();
		else if (is<TransformNode>(node))
			name = nameOfNode<TransformNode>();
		else if (is<GroupNode>(node))
			name = nameOfNode<GroupNode>();

		auto       color = colorFromNode(name);
		const auto id    = node->id();

		stream << "    \"node" << id << "\" [label=\"id: " << id
		       << " type: " << name << "\", style=filled,color=\"" << color
		       << "\"];\n";

		stream << "    \"node" << id << "\" -> {\n";
		for (const auto &child : node->children()) {
			stream << "        \"node" << child.get().id() << "\" ;\n";

			if (std::find(
			        std::begin(node_done), std::end(node_done), &(child.get()))
			    == std::end(node_done))
				node_stack.emplace(&(child.get()));
		}
		stream << "    } [color=seagreen]\n\n";

		node_done.emplace_back(node);
	}

	stream << "}" << std::flush;

	stream.close();
}

/////////////////////////////////////
/////////////////////////////////////
void Scene::exportSceneTree(const _std::filesystem::path &filepath) const {
	m_tree.exportSceneTree(filepath, colorFromNode);
}
