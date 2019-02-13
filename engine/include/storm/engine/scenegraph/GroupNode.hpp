#pragma once

#include <storm/engine/scenegraph/SceneNode.hpp>

namespace storm::engine {
	class STORM_PUBLIC GroupNode : public SceneNode {
	public:
		SUR_Object(GroupNode)

		    ~GroupNode() override;

		GroupNode(GroupNode &&);
		GroupNode &operator=(GroupNode &&);

	protected:
		explicit GroupNode(Scene &graph);

		DirtyType dirtyValue() const noexcept override;

		friend class Scene;
	};
}
