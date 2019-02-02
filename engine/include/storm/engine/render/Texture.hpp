// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/Utils.hpp>

#include <storm/image/Image.hpp>

#include <storm/core/Pimpl.hpp>
#include <storm/core/Memory.hpp>

namespace storm::engine {
	class TextureImpl;
	class Texture {
		public:
			Unique_Object(Texture)
			
			explicit Texture(const Device &device, const image::Image &image);
			~Texture();
			
			Texture(Texture &&);
			Texture &operator=(Texture &&);
			
			IMPLEMENTATION(TextureImpl)
		private:
			core::Pimpl<TextureImpl> m_impl;
	};
}
