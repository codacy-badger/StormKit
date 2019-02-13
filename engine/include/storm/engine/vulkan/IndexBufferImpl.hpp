// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/render/IndexBuffer.hpp>
#include <storm/engine/vulkan/DeviceImpl.hpp>

namespace storm::engine {
	class IndexBufferImpl {
	public:
		explicit IndexBufferImpl(
		    const Device &device, std::size_t size, std::size_t alignement);
		~IndexBufferImpl();

		IndexBufferImpl(IndexBufferImpl &&);

		void reset();

		std::size_t addData(const std::byte *data, std::size_t size);
		void        updateData(
		           const std::byte *data, std::size_t size, std::ptrdiff_t offset);

		std::byte *map();
		std::byte *map(std::size_t size, std::ptrdiff_t offset);
		void       unmap();

		inline const BackedVkBuffer &backedVkBuffer() const noexcept;

	private:
		BackedVkBuffer m_buffer;

		std::size_t    m_size;
		std::size_t    m_alignement;
		std::ptrdiff_t m_current_offset;

		std::byte *m_mapped_data;

		const DeviceImpl &m_device;
	};
}

#include "IndexBufferImpl.inl"
