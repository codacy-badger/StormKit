#pragma once

#include <memory>

#include <storm/core/Platform.hpp>
#include <storm/engine/scenegraph/SceneNode.hpp>
#include <storm/engine/graphics/DrawableCache.hpp>

namespace storm::engine {
	class SceneTree;
	class STORM_PUBLIC SceneGraph {
		public:
			SUR_Object(SceneGraph)

			explicit SceneGraph();
			~SceneGraph();

			SceneNode &root() noexcept { return *m_root; }

			//SceneTree &updateCache(DrawableCache &cache);
		private:
			SceneNode::SPtr            m_root;
			std::shared_ptr<SceneTree> m_tree;
	};
}
