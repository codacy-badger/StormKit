// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/render/UniformBuffer.hpp>
#include <storm/engine/vulkan/UniformBufferImpl.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
UniformBuffer::UniformBuffer(const Device &device, Description description)
	: m_impl{device, std::move(description)} {

}

/////////////////////////////////////
/////////////////////////////////////
UniformBuffer::~UniformBuffer() = default;

/////////////////////////////////////
/////////////////////////////////////
UniformBuffer::UniformBuffer(UniformBuffer &&) = default;

/////////////////////////////////////
/////////////////////////////////////
UniformBuffer &UniformBuffer::operator=(UniformBuffer &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void UniformBuffer::reset() {
	m_impl->reset();
}

/////////////////////////////////////
/////////////////////////////////////
std::size_t UniformBuffer::addData(const std::byte *data, std::size_t size) {
	return m_impl->addData(data, size);
}

/////////////////////////////////////
/////////////////////////////////////
void UniformBuffer::updateData(const std::byte *data, std::size_t size, std::ptrdiff_t offset) {
	m_impl->updateData(data, size, offset);
}

/////////////////////////////////////
/////////////////////////////////////
std::byte *UniformBuffer::map() {
	return m_impl->map();
}

/////////////////////////////////////
/////////////////////////////////////
std::byte *UniformBuffer::map(std::size_t size, std::ptrdiff_t offset) {
	return m_impl->map(size, offset);
}

/////////////////////////////////////
/////////////////////////////////////
void UniformBuffer::unmap() {
	m_impl->unmap();
}

/////////////////////////////////////
/////////////////////////////////////
const UniformBuffer::Description &UniformBuffer::description() const noexcept {
	return m_impl->description();
}
