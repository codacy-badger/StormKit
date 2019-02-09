#pragma once

#include <vector>

#include <storm/engine/graphics/Geometry.hpp>
#include <storm/engine/scenegraph/Node.hpp>

namespace storm::engine {
	class STORM_PUBLIC GeoNode : public Node {
		public:
			SUR_Object(GeoNode)

			using HoldedType = Geometry;

			~GeoNode() override = default;

			inline const HoldedType &data() const noexcept { return geometry(); }
			inline const HoldedType &geometry() const noexcept { return m_geometry; }
			template <typename T = HoldedType, typename = std::enable_if_t<std::is_same_v<std::decay_t<std::remove_cv_t<T>>, HoldedType>>>
			inline void setGeometry(T&& geometry) noexcept { m_geometry = std::forward<T>(geometry); notify(NodeEvent::UPDATED); }
		protected:
			explicit GeoNode() = default;

			std::uint32_t dirtyValue() const noexcept override;

		private:
			HoldedType m_geometry;
	};
}
