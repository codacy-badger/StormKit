// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <functional>
#include <string>
#include <type_traits>

namespace storm::core {
	template <typename T>
	struct enum_hash {
		constexpr std::size_t operator()(T val) const noexcept {
			using underlying_type = std::underlying_type_t<T>;

			auto val_c = static_cast<underlying_type>(val);

			return std::hash<underlying_type> {}(val_c);
		}

	private:
		using sfinae = std::enable_if_t<std::is_enum_v<T>>;
	};

	template <class T>
	constexpr inline void hash_combine(std::size_t &seed, const T &v) {
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	template <typename Result>
	constexpr inline Result ComponentHash(const char *str, std::size_t size) {
		return size == 0 ? 0xcbf29ce484222325UL
		                 : (static_cast<std::size_t>(str[0])
		                       ^ ComponentHash<Result>(str + 1, size - 1))
		                       * 0x100000001b3UL;
	}

	template <typename Result>
	inline Result ComponentHash(const std::string &str) {
		return ComponentHash<Result>(str.c_str(), std::size(str));
	}

	constexpr inline std::uint64_t operator"" _hash(
	    const char *str, std::size_t size) {
		return ComponentHash<std::uint64_t>(str, size);
	}
}

namespace std {
	template <typename T>
	struct hash<std::vector<T>> {
		std::size_t operator()(const std::vector<T> &in) const {
			auto        size = std::size(in);
			std::size_t seed = 0;
			for (size_t i = 0; i < size; i++)
				storm::core::hash_combine(seed, in[i]);

			return seed;
		}
	};
}

#define HASH_FUNC(x)                                                           \
	template <>                                                                \
	struct hash<x> {                                                           \
		std::size_t operator()(const x &) const;                               \
	};                                                                         \
	template <>                                                                \
	struct equal_to<x> {                                                       \
		bool operator()(const x &, const x &second) const noexcept;            \
	};
