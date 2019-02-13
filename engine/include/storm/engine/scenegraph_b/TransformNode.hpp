#pragma once

#include <storm/engine/graphics/Transform.hpp>
#include <storm/engine/scenegraph/Node.hpp>

namespace storm::engine {
	class STORM_PUBLIC TransformNode : public Node {
	public:
		SUR_Object(TransformNode) using HoldedType = Transform;

		~TransformNode() override = default;

		inline const HoldedType &data() const noexcept { return transform(); }
		inline const HoldedType &transform() const noexcept {
			return m_transform;
		}

		template <typename T = HoldedType,
		    typename         = std::enable_if_t<
                std::is_same_v<std::decay_t<std::remove_cv_t<T>>, HoldedType>>>
		inline void setTransform(T &&transform) noexcept {
			m_transform = std::forward<T>(transform);
			notify(NodeEvent::UPDATED);
		}

	protected:
		explicit TransformNode() = default;

		std::uint32_t dirtyValue() const noexcept override;

	private:
		HoldedType m_transform;
	};
}
