// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/Utils.hpp>
#include <storm/engine/render/Types.hpp>

#include <storm/image/Image.hpp>

#include <storm/core/Pimpl.hpp>
#include <storm/core/Memory.hpp>

namespace storm::engine {
	struct TextureDescription {
		std::size_t mip_level;
		Format      format;
		uvec3       size;
	};

	class TextureImpl;
	class Texture {
		public:
			Unique_Object(Texture)
			
			explicit Texture(const Device &device, const image::Image &image);
			explicit Texture(const Device &device, TextureDescription description);
			~Texture();
			
			Texture(Texture &&);
			Texture &operator=(Texture &&);

			const image::Image &image() const noexcept;
			
			IMPLEMENTATION(TextureImpl)
		private:
			core::Pimpl<TextureImpl> m_impl;
	};
}
