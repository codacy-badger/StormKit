// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/render/Texture.hpp>
#include <storm/engine/vulkan/TextureImpl.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
Texture::Texture(const Device &device, const image::Image &image) 
	: m_impl{device, image} {
	
}

/////////////////////////////////////
/////////////////////////////////////
Texture::~Texture() = default;

/////////////////////////////////////
/////////////////////////////////////
Texture::Texture(Texture &&) = default;

/////////////////////////////////////
/////////////////////////////////////
Texture &Texture::operator=(Texture &&) = default;
