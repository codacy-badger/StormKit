#pragma once

#include <storm/engine/scenegraph/Node.hpp>
#include <storm/engine/graphics/Material.hpp>

namespace storm::engine {
	class STORM_PUBLIC MaterialNode : public Node {
		public:
			SUR_Object(MaterialNode)
			using HoldedType = Material;

			~MaterialNode() override = default;

			inline const HoldedType &data() const noexcept { return material(); }
			inline const HoldedType &material() const noexcept { return m_material; }
			template <typename T = HoldedType, typename = std::enable_if_t<std::is_same_v<std::decay_t<std::remove_cv_t<T>>, HoldedType>>>
			inline void setMaterial(T&& material) noexcept { m_material = std::forward<T>(material); notify(NodeEvent::UPDATED); }
		protected:
			explicit MaterialNode();

			std::uint32_t dirtyValue() const noexcept override;

		private:
			HoldedType m_material;
	};
}
