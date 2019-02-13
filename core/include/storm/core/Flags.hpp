// Copryright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

/// \file core
/// \author Arthapz

#pragma once
// From
// https://www.justsoftwaresolutions.co.uk/cplusplus/using-enum-classes-as-bitfields.html

#include <type_traits>

/// \exclude
template <typename E>
struct enable_bitmask_operators {
	static const bool enable = false;
};

/// \exclude
template <typename E>
constexpr typename std::enable_if<enable_bitmask_operators<E>::enable, E>::type
operator|(E lhs, E rhs) {
	typedef typename std::underlying_type<E>::type underlying;
	return static_cast<E>(
	    static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

/// \exclude
template <typename E>
constexpr typename std::enable_if<enable_bitmask_operators<E>::enable, E>::type
operator&(E lhs, E rhs) {
	typedef typename std::underlying_type<E>::type underlying;
	return static_cast<E>(
	    static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

/// \exclude
template <typename E>
constexpr typename std::enable_if<enable_bitmask_operators<E>::enable, E>::type
operator^(E lhs, E rhs) {
	typedef typename std::underlying_type<E>::type underlying;
	return static_cast<E>(
	    static_cast<underlying>(lhs) ^ static_cast<underlying>(rhs));
}

/// \exclude
template <typename E>
constexpr typename std::enable_if<enable_bitmask_operators<E>::enable, E>::type
operator~(E lhs) {
	typedef typename std::underlying_type<E>::type underlying;
	return static_cast<E>(~static_cast<underlying>(lhs));
}

/// \exclude
template <typename E>
constexpr
    typename std::enable_if<enable_bitmask_operators<E>::enable, E &>::type
    operator|=(E &lhs, E rhs) {
	typedef typename std::underlying_type<E>::type underlying;
	lhs = static_cast<E>(
	    static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
	return lhs;
}

/// \exclude
template <typename E>
constexpr
    typename std::enable_if<enable_bitmask_operators<E>::enable, E &>::type
    operator&=(E &lhs, E rhs) {
	typedef typename std::underlying_type<E>::type underlying;
	lhs = static_cast<E>(
	    static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
	return lhs;
}

/// \exclude
template <typename E>
constexpr
    typename std::enable_if<enable_bitmask_operators<E>::enable, E &>::type
    operator^=(E &lhs, E rhs) {
	typedef typename std::underlying_type<E>::type underlying;
	lhs = static_cast<E>(
	    static_cast<underlying>(lhs) ^ static_cast<underlying>(rhs));
	return lhs;
}

#define FLAG_ENUM(x)                                                           \
	template <>                                                                \
	struct enable_bitmask_operators<x> {                                       \
		static const bool enable = true;                                       \
	};
