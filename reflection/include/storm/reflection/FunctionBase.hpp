#pragma once

#include <string>

namespace storm::reflection {
	class FunctionBase {
		public:
			explicit FunctionBase() = default;

			inline virtual const std::string &name() const noexcept = 0;
	};
}
