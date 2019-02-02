#pragma once

#include <storm/core/NonInstanciable.hpp>
#include <storm/core/Memory.hpp>

#include <memory>
#include <string>
#include <cstdint>

namespace storm::entities {
	struct Component {
		Shared_Object(Component)
		Unique_Object(Component)

		explicit Component() = default;

		Component(const Component &) = default;
		Component &operator=(const Component &) = default;

		Component(Component &&) = default;
		Component &operator=(Component &&) = default;

		virtual ~Component() = default;
		using Type = std::uint64_t;

		static constexpr Type INVALID_TYPE = 0;
		static constexpr Type TYPE = INVALID_TYPE;
	};

	constexpr Component::Type ComponentHash(const char *str, std::size_t sz) {
	  return sz == 0 ? 0xcbf29ce484222325UL : (std::size_t(str[0]) ^ ComponentHash(str + 1, sz - 1)) * 0x100000001b3UL;
	}

    inline Component::Type ComponentHash(const std::string& str) {
	  return ComponentHash(str.c_str(), str.size());
	}
}

constexpr storm::entities::Component::Type operator"" _type(const char *str, std::size_t sz) {
  return storm::entities::ComponentHash(str, sz);
}
