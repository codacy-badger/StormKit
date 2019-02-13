#pragma once

#include <stdexcept>
#include <string>

namespace storm::reflection {
	class IncorrectParametersSignatureException final
	    : public std::runtime_error {
	public:
		explicit IncorrectParametersSignatureException(
		    const std::string &func_name)
		    : std::runtime_error("Invalid signature on function" + func_name) {}
	};

	class NotRegisteredException final : public std::runtime_error {
	public:
		explicit NotRegisteredException(const std::string &func_name)
		    : std::runtime_error(func_name + " Not registered") {}
	};
}
