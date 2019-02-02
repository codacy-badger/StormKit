#pragma once

#include <storm/engine/scenegraph/Node.hpp>
#include <storm/engine/render/Types.hpp>

namespace storm::engine {
	class STORM_PUBLIC SceneNode : public Node {
		public:
			SUR_Object(SceneNode)

			using HoldedType = mat4;

			~SceneNode() override = default;

			inline const mat4 &data() const noexcept { return projection(); }
			inline const mat4 &projection() const noexcept { return m_projection; }
			template <typename T = HoldedType, typename = std::enable_if_t<std::is_same_v<std::decay_t<std::remove_cv_t<T>>, mat4>>>
			inline void setProjection(T&& projection) noexcept { m_projection = std::forward<T>(projection); notify(NodeEvent::UPDATED); }
		protected:
			explicit SceneNode();

			std::uint32_t dirtyValue() const noexcept override;

		private:
			mat4 m_projection;
	};
}
