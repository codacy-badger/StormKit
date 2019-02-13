// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/render/UniformBuffer.hpp>
#include <storm/engine/vulkan/DeviceImpl.hpp>

namespace storm::engine {
	class UniformBufferImpl {
	public:
		explicit UniformBufferImpl(
		    const Device &device, UniformBuffer::Description description);
		~UniformBufferImpl();

		UniformBufferImpl(UniformBufferImpl &&);

		void reset();

		std::size_t addData(const std::byte *data, std::size_t size);
		void        updateData(
		           const std::byte *data, std::size_t size, std::ptrdiff_t offset);

		std::byte *map();
		std::byte *map(std::size_t size, std::ptrdiff_t offset);
		void       unmap();

		inline const UniformBuffer::Description &description() const noexcept;
		inline const BackedVkBuffer &backedVkBuffer() const noexcept;

	private:
		BackedVkBuffer m_buffer;

		UniformBuffer::Description m_description;
		std::ptrdiff_t             m_current_offset;

		std::byte *m_mapped_data;

		const DeviceImpl &m_device;
	};
}

#include "UniformBufferImpl.inl"
