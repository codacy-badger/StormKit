#pragma once

#include <storm/engine/render/Types.hpp>
#include <storm/engine/scenegraph/Node.hpp>

namespace storm::engine {
	class STORM_PUBLIC ViewNode : public Node {
	public:
		SUR_Object(ViewNode)

		    using HoldedType = mat4;

		~ViewNode() override = default;

		inline const mat4 &viewMatrix() const noexcept { return m_view_matrix; }
		template <typename T = mat4,
		    typename         = std::enable_if_t<
                std::is_same_v<std::decay_t<std::remove_cv_t<T>>, mat4>>>
		inline void setViewMatrix(T &&view) noexcept {
			m_view_matrix = std::forward<T>(view);
			commit();
		}

		inline HoldedType data() const noexcept { return m_view_matrix; }

		void commit() noexcept;

	protected:
		explicit ViewNode();

		std::uint32_t dirtyValue() const noexcept override;

	private:
		mat4 m_view_matrix;
	};
}

#include "ViewNode.hpp"
