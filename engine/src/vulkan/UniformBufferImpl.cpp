// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/core/Assert.hpp>
#include <storm/engine/vulkan/UniformBufferImpl.hpp>

using namespace storm::engine;

// todo implement stagging buffer
/////////////////////////////////////
/////////////////////////////////////
UniformBufferImpl::UniformBufferImpl(
    const Device &device, UniformBuffer::Description description)
    : m_description {std::move(description)}, m_current_offset {0},
      m_mapped_data {nullptr}, m_device {device.implementation()} {
	m_buffer
	    = m_device.createBackedVkBuffer(vk::BufferUsageFlagBits::eUniformBuffer,
	        vk::MemoryPropertyFlagBits::eHostCoherent
	            | vk::MemoryPropertyFlagBits::eHostVisible,
	        m_description.size);
}

/////////////////////////////////////
/////////////////////////////////////
UniformBufferImpl::~UniformBufferImpl() = default;

/////////////////////////////////////
/////////////////////////////////////
UniformBufferImpl::UniformBufferImpl(UniformBufferImpl &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void UniformBufferImpl::reset() { m_current_offset = 0; }

/////////////////////////////////////
/////////////////////////////////////
std::size_t UniformBufferImpl::addData(
    const std::byte *data, std::size_t size) {
	auto offset = m_current_offset;

	updateData(data, size, offset);

	m_current_offset += size;

	if (m_description.alignement > 0)
		m_current_offset += m_description.alignement
		                    - m_current_offset % m_description.alignement;

	return offset;
}

/////////////////////////////////////
/////////////////////////////////////
void UniformBufferImpl::updateData(
    const std::byte *data, std::size_t size, std::ptrdiff_t offset) {
	ASSERT((offset + size) <= m_description.size,
	    "Offset + size cannot be > to the buffer size");

	auto mem = map(size, offset);

	std::memcpy(mem, data, size);

	unmap();
}

/////////////////////////////////////
/////////////////////////////////////
std::byte *UniformBufferImpl::map() { return map(m_description.size, 0); }

/////////////////////////////////////
/////////////////////////////////////
std::byte *UniformBufferImpl::map(std::size_t size, std::ptrdiff_t offset) {
	m_mapped_data = reinterpret_cast<std::byte *>(
	    m_device.vkDevice().mapMemory(m_buffer.memory.get(), offset, size));
	return m_mapped_data;
}

/////////////////////////////////////
/////////////////////////////////////
void UniformBufferImpl::unmap() {
	m_mapped_data = nullptr;
	m_device.vkDevice().unmapMemory(m_buffer.memory.get());
}
