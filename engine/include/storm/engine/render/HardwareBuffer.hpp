// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/Memory.hpp>
#include <storm/core/Pimpl.hpp>

#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/Utils.hpp>
#include <storm/engine/render/Types.hpp>

namespace storm::engine {
	class HardwareBufferImpl;
	class HardwareBuffer {
		public:
			struct Description {
				std::size_t size;
				std::size_t alignement;
                BufferUsageFlag usage;
			};

			Unique_Object(HardwareBuffer)

			explicit HardwareBuffer(const Device &device, Description description);
			~HardwareBuffer();

			HardwareBuffer(HardwareBuffer &&);
			HardwareBuffer &operator=(HardwareBuffer &&);

			void reset();

			void setData(const std::byte *data, std::size_t size, std::ptrdiff_t offset);

			template <class Container>
			inline void setData(Container &&container, std::ptrdiff_t offset = 0u);

			std::byte *map();
			std::byte *map(std::size_t size, std::ptrdiff_t offset);
			void unmap();

			const HardwareBuffer::Description &description();

			IMPLEMENTATION(HardwareBufferImpl)
		private:
			core::Pimpl<HardwareBufferImpl> m_impl;
	};
}

#include "HardwareBuffer.tpp"
