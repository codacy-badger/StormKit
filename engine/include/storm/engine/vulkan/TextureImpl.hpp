// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/NonCopyable.hpp>

#include <storm/engine/vulkan/DeviceImpl.hpp>

#include <storm/image/Image.hpp>

namespace storm::engine {
	class Device;
	class TextureImpl : public core::NonCopyable {
		public:
			explicit TextureImpl(const Device &device, const image::Image &image);
			~TextureImpl();
			
			TextureImpl(TextureImpl &&);
			
			inline const BackedVkTexture &backedVkTexture() const noexcept;
		private:
			BackedVkTexture m_texture;
			
			image::Image m_image;
			
			const Device &m_device;
	};
}

#include "TextureImpl.inl"
