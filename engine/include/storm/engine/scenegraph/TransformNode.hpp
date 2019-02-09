#pragma once

#include <storm/engine/graphics/Transform.hpp>

#include <storm/engine/scenegraph/SceneNode.hpp>

namespace storm::engine {
	class STORM_PUBLIC TransformNode : public SceneNode {
		public:
			SUR_Object(TransformNode)

			using DataType = Transform;

			~TransformNode() override;

			TransformNode(TransformNode &&);
			TransformNode &operator=(TransformNode &&);

			inline const DataType &data()      const noexcept;
			inline void setTransform(Transform transform) noexcept;
		protected:
			explicit TransformNode(Scene &graph);

			DirtyType dirtyValue() const noexcept override;

			friend class Scene;
		private:
			DataType m_transform;
	};
}

#include "TransformNode.inl"
