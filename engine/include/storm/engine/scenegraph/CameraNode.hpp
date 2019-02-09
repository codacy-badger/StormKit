#pragma once

#include <storm/engine/scenegraph/SceneNode.hpp>

#include <storm/engine/render/Types.hpp>

namespace storm::engine {
	class STORM_PUBLIC CameraNode : public SceneNode {
		public:
			SUR_Object(CameraNode)

			struct Camera {
				mat4 view;
				mat4 projection;

				bool is_view_dirty;
				bool is_projection_dirty;
			};

			using DataType = Camera;

			~CameraNode() override;

			CameraNode(CameraNode &&);
			CameraNode &operator=(CameraNode &&);

			inline const mat4 &view() const noexcept;
			inline void setView(mat4 view) noexcept;

			inline const mat4 &projection() const noexcept;
			inline void setProjection(mat4 projection) noexcept;

			inline const DataType &data() const noexcept;

			void commit() noexcept;
		protected:
			explicit CameraNode(Scene &graph);

			DirtyType dirtyValue() const noexcept override;

			friend class Scene;
		private:
			DataType m_camera;
	};
}

#include "CameraNode.inl"
