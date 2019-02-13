// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/core/Assert.hpp>
#include <storm/engine/vulkan/IndexBufferImpl.hpp>

using namespace storm::engine;

// todo implement stagging buffer
/////////////////////////////////////
/////////////////////////////////////
IndexBufferImpl::IndexBufferImpl(
    const Device &device, std::size_t size, std::size_t alignement)
    : m_size {size}, m_alignement {alignement}, m_current_offset {0},
      m_mapped_data {nullptr}, m_device {device.implementation()} {
	m_buffer
	    = m_device.createBackedVkBuffer(vk::BufferUsageFlagBits::eIndexBuffer,
	        vk::MemoryPropertyFlagBits::eHostCoherent
	            | vk::MemoryPropertyFlagBits::eHostVisible,
	        size);
}

/////////////////////////////////////
/////////////////////////////////////
IndexBufferImpl::~IndexBufferImpl() = default;

/////////////////////////////////////
/////////////////////////////////////
IndexBufferImpl::IndexBufferImpl(IndexBufferImpl &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void IndexBufferImpl::reset() { m_current_offset = 0; }

/////////////////////////////////////
/////////////////////////////////////
std::size_t IndexBufferImpl::addData(const std::byte *data, std::size_t size) {
	auto offset = m_current_offset;

	updateData(data, size, offset);

	m_current_offset += size;

	if (m_alignement > 0)
		m_current_offset += m_alignement - m_current_offset % m_alignement;

	return offset;
}

/////////////////////////////////////
/////////////////////////////////////
void IndexBufferImpl::updateData(
    const std::byte *data, std::size_t size, std::ptrdiff_t offset) {
	ASSERT((offset + size) <= m_size,
	    "Offset + size cannot be > to the buffer size");

	auto mem = map(size, offset);

	std::memcpy(mem, data, size);

	unmap();
}

/////////////////////////////////////
/////////////////////////////////////
std::byte *IndexBufferImpl::map() { return map(m_size, 0); }

/////////////////////////////////////
/////////////////////////////////////
std::byte *IndexBufferImpl::map(std::size_t size, std::ptrdiff_t offset) {
	m_mapped_data = reinterpret_cast<std::byte *>(
	    m_device.vkDevice().mapMemory(m_buffer.memory.get(), offset, size));
	return m_mapped_data;
}

/////////////////////////////////////
/////////////////////////////////////
void IndexBufferImpl::unmap() { m_mapped_data = nullptr; }
