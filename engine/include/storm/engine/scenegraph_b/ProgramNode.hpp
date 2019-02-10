#pragma once

#include <storm/engine/scenegraph/Node.hpp>

namespace storm::engine {
	class STORM_PUBLIC ProgramNode : public Node {
		public:
			SUR_Object(ProgramNode)

			using HoldedType = std::string;

			~ProgramNode() override = default;

			inline const HoldedType &data() const noexcept { return programID(); }
			inline const HoldedType &programID() const noexcept { return m_program_id; }

			template <typename T = HoldedType, typename = std::enable_if_t<std::is_same_v<std::decay_t<std::remove_cv_t<T>>, HoldedType> || std::is_constructible_v<HoldedType, T>>>
			inline void setProgramID(T&& id) noexcept { m_program_id = std::forward<T>(id); notify(NodeEvent::UPDATED); }
		protected:
			explicit ProgramNode() = default;

			std::uint32_t dirtyValue() const noexcept override;

		private:
			HoldedType m_program_id;
	};
}
