#pragma once

#include <storm/engine/scenegraph/Node.hpp>

namespace storm::engine {
	class STORM_PUBLIC GroupNode : public Node {
		public:
			SUR_Object(GroupNode)

			~GroupNode() override = default;

		protected:
			explicit GroupNode();

			std::uint32_t dirtyValue() const noexcept override;
	};
}
