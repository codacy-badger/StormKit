// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <utility>

#include <glm/gtc/type_ptr.hpp>

#define IMPLEMENTATION(type) \
	inline const type &implementation() const noexcept { \
		return *m_impl; \
	} \
	inline type &implementation() noexcept { \
		return *m_impl; \
	}

#define BACKEND_NATIVE_OBJECT_NAMED(method, type, member) \
	inline const type &method() const noexcept { \
		return std::as_const(member); \
	} \
	inline type &method() noexcept { \
		return member; \
	}

#define BACKEND_NATIVE_OBJECT(type, member) BACKEND_NATIVE_OBJECT_NAMED(nativeObject, type, member)


#define BACKEND_NATIVE_OBJECT_POINTER(Type, member) \
	inline const Type &nativeObject() const noexcept { \
		return std::as_const(member.get()); \
	} \
	inline Type &nativeObject() noexcept { \
		return member.get(); \
	}

template <typename T>
inline auto value_ptr(T &value) {
	if constexpr(std::is_arithmetic_v<std::remove_reference_t<T>>)
		return &value;
	else
		return glm::value_ptr(value);
}
