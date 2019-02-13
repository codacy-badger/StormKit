// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/graphics/Resource.hpp>

namespace storm::engine {
	template <>
	Texture::Ptr realize(
	    const Device &device, Texture::Description description) {
		return Texture::makeUnique(device, std::move(description));
	}

	template <>
	UniformBuffer::Ptr realize(
	    const Device &device, UniformBuffer::Description description) {
		return UniformBuffer::makeUnique(device, std::move(description));
	}

	template <>
	Framebuffer::Ptr realize(
	    const Device &device, Framebuffer::Description description) {
		return nullptr;
	}
}
