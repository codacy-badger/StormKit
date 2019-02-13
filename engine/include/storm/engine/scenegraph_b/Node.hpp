#pragma once

#include <storm/core/Any.hpp>
#include <storm/core/Memory.hpp>
#include <storm/core/NonCopyable.hpp>
#include <storm/core/NonInstanciable.hpp>
#include <storm/engine/scenegraph/NodeData.hpp>
#include <storm/tools/Subject.hpp>
#include <vector>

namespace storm::engine {
	class SceneGraph;
	class STORM_PUBLIC Node
	    : public core::NonCopyable
	    , public std::enable_shared_from_this<Node>
	    , public NodeSubject {
	public:
		SUR_Object(Node)

		    virtual ~Node()
		    = 0;

		void addChild(const Node::Ptr &child);
		void removeChild(const Node::Ptr &child);

		inline const NodeArray &parents() const noexcept { return m_parents; }
		inline const NodeArray &children() const noexcept { return m_childs; }

		void setObserver(NodeObserver::Ptr &&observer);

	protected:
		explicit Node();

		void                  notify(NodeEvent event) noexcept;
		virtual std::uint32_t dirtyValue() const noexcept = 0;

		friend class SceneTree;
		friend class SceneGraph;

	private:
		SceneGraph *m_graph;
		NodeArray   m_parents;
		NodeArray   m_childs;
	};
}
