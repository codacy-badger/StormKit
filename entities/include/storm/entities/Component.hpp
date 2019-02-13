#pragma once

#include <cstdint>
#include <memory>
#include <storm/core/Hash.hpp>
#include <storm/core/Memory.hpp>
#include <storm/core/NonInstanciable.hpp>
#include <string>

namespace storm::entities {
	struct Component {
		Shared_Object(Component) Unique_Object(Component)

		    explicit Component()
		    = default;

		Component(const Component &) = default;
		Component &operator=(const Component &) = default;

		Component(Component &&) = default;
		Component &operator=(Component &&) = default;

		virtual ~Component() = default;
		using Type           = std::uint64_t;

		static constexpr Type INVALID_TYPE = 0;
		static constexpr Type TYPE         = INVALID_TYPE;
	};

	constexpr Component::Type operator"" _type(
	    const char *str, std::size_t size) {
		return core::ComponentHash<Component::Type>(str, size);
	}
}
