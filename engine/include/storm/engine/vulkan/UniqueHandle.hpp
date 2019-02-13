#pragma once

#include <vulkan/vulkan.hpp>

namespace storm::engine {
	template <typename T>
	using UniqueHandle = vk::UniqueHandle<T, vk::DispatchLoaderStatic>;

	template <typename T>
	inline auto vkMakeVersion(T &&major, T &&minor, T &&patch) {
		return (((major) << 22u) | ((minor) << 12u) | (patch));
	}

	template <typename T>
	inline auto vkVersionMajor(T &&version) {
		return static_cast<std::remove_reference_t<T>>(
		    static_cast<std::uint32_t>(version >> 22u));
	}

	template <typename T>
	inline auto vkVersionMinor(T &&version) {
		return static_cast<std::remove_reference_t<T>>(
		    static_cast<std::uint32_t>((version >> 12u) & 0x3ffu));
	}

	template <typename T>
	inline auto vkVersionPatch(T &&version) {
		return static_cast<std::remove_reference_t<T>>(
		    static_cast<std::uint32_t>(version & 0xfffu));
	}
}
