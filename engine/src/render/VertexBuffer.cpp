// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/render/VertexBuffer.hpp>
#include <storm/engine/vulkan/VertexBufferImpl.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
VertexBuffer::VertexBuffer(const Device &device, std::size_t size, std::size_t alignement)
	: m_impl{device, size, alignement} {

}

/////////////////////////////////////
/////////////////////////////////////
VertexBuffer::~VertexBuffer() = default;

/////////////////////////////////////
/////////////////////////////////////
VertexBuffer::VertexBuffer(VertexBuffer &&) = default;

/////////////////////////////////////
/////////////////////////////////////
VertexBuffer &VertexBuffer::operator=(VertexBuffer &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void VertexBuffer::reset() {
	m_impl->reset();
}

/////////////////////////////////////
/////////////////////////////////////
std::size_t VertexBuffer::addData(const std::byte *data, std::size_t size) {
	return m_impl->addData(data, size);
}

/////////////////////////////////////
/////////////////////////////////////
void VertexBuffer::updateData(const std::byte *data, std::size_t size, std::ptrdiff_t offset) {
	m_impl->updateData(data, size, offset);
}

/////////////////////////////////////
/////////////////////////////////////
std::byte *VertexBuffer::map() {
	return m_impl->map();
}

/////////////////////////////////////
/////////////////////////////////////
std::byte *VertexBuffer::map(std::size_t size, std::ptrdiff_t offset) {
	return m_impl->map(size, offset);
}

/////////////////////////////////////
/////////////////////////////////////
void VertexBuffer::unmap() {
	m_impl->unmap();
}
