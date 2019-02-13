#pragma once

#include <cstdint>
#include <string>

namespace storm::engine {
	struct ContextSettings {
		std::string app_name;
		struct {
			std::uint32_t major;
			std::uint32_t minor;
			std::uint32_t patch;
		} app_version;

		bool enable_validation = false;
	};
}
