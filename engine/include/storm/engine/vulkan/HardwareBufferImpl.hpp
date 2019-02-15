// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/vulkan/DeviceImpl.hpp>

#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/HardwareBuffer.hpp>

namespace storm::engine {
	class HardwareBufferImpl {
		public:
			explicit HardwareBufferImpl(const Device &device, HardwareBuffer::Description description);
			~HardwareBufferImpl();

			HardwareBufferImpl(HardwareBufferImpl &&);

			void reset();

			void setData(const std::byte *data, std::size_t size, std::ptrdiff_t offset = 0u);

			std::byte *map();
			std::byte *map(std::size_t size, std::ptrdiff_t offset);
			void unmap();

			void flush();

			inline const HardwareBuffer::Description &description();
			inline const BackedVkBuffer &backedVkBuffer() const noexcept;
		private:
			const Device &m_device;

			HardwareBuffer::Description m_description;

			BackedVkBuffer m_buffer;
			BackedVkBuffer m_stagging_buffer;

			std::ptrdiff_t m_current_offset;
			std::byte     *m_mapped_data;
	};
}

#include "HardwareBufferImpl.inl"
