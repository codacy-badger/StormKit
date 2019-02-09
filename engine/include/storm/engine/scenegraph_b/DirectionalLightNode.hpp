#pragma once

#include <storm/engine/scenegraph/Node.hpp>
#include <storm/engine/graphics/DirectionalLight.hpp>

namespace storm::engine {
	class STORM_PUBLIC DirectionalLightNode : public Node {
		public:
			Shared_Object(DirectionalLightNode)
			using HoldedType = DirectionalLight;

			explicit DirectionalLightNode();

			~DirectionalLightNode() override = default;

			inline const HoldedType &data() const noexcept { return light(); }
			inline const HoldedType &light() const noexcept { return m_light; }
			template <typename T = HoldedType, typename = std::enable_if_t<std::is_same_v<std::decay_t<std::remove_cv_t<T>>, HoldedType>>>
			inline void setLight(T&& light) noexcept { m_light = std::forward<T>(light); notify(NodeEvent::UPDATED); }
		protected:
			std::uint32_t dirtyValue() const noexcept override;

		private:
			HoldedType m_light;
	};
}
