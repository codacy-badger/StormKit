#pragma once

#include <vector>
#include <memory>

#include <storm/core/Any.hpp>

#include <storm/tools/Observer.hpp>
#include <storm/tools/Subject.hpp>

namespace storm::engine {
	class Node;
	using NodeArray = std::vector<std::shared_ptr<Node>>;

	enum class NodeEvent {
		ADDED = 1, REMOVED = 2, UPDATED = 3
	};
	using NodePayload  = std::pair<std::shared_ptr<Node>, std::uint32_t>;
	using NodeObserver = tools::Observer<NodeEvent, NodePayload>;
	using NodeSubject  = tools::Subject<NodeEvent, NodePayload>;
}

