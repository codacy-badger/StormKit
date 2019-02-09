#pragma once

#include <vector>
#include <memory>

#include <storm/core/Any.hpp>

#include <storm/tools/Observer.hpp>
#include <storm/tools/Subject.hpp>

namespace storm::engine {
	class SceneNode;
	using SceneNodeArray = std::vector<std::reference_wrapper<SceneNode>>;

	enum class NodeEvent {
		ADDED = 1, REMOVED = 2, UPDATED = 3
	};

	struct NodePayload {
		std::reference_wrapper<SceneNode> node;
		std::uint32_t index;
	};

	using NodeObserver = tools::Observer<NodeEvent, NodePayload>;
	using NodeSubject  = tools::Subject<NodeEvent, NodePayload>;
}

