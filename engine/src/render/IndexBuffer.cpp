// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/render/IndexBuffer.hpp>
#include <storm/engine/vulkan/IndexBufferImpl.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
IndexBuffer::IndexBuffer(
    const Device &device, std::size_t size, std::size_t alignement)
    : m_impl {device, size, alignement} {}

/////////////////////////////////////
/////////////////////////////////////
IndexBuffer::~IndexBuffer() = default;

/////////////////////////////////////
/////////////////////////////////////
IndexBuffer::IndexBuffer(IndexBuffer &&) = default;

/////////////////////////////////////
/////////////////////////////////////
IndexBuffer &IndexBuffer::operator=(IndexBuffer &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void IndexBuffer::reset() { m_impl->reset(); }

/////////////////////////////////////
/////////////////////////////////////
std::size_t IndexBuffer::addData(const std::byte *data, std::size_t size) {
	return m_impl->addData(data, size);
}

/////////////////////////////////////
/////////////////////////////////////
void IndexBuffer::updateData(
    const std::byte *data, std::size_t size, std::ptrdiff_t offset) {
	m_impl->updateData(data, size, offset);
}

/////////////////////////////////////
/////////////////////////////////////
std::byte *IndexBuffer::map() { return m_impl->map(); }

/////////////////////////////////////
/////////////////////////////////////
std::byte *IndexBuffer::map(std::size_t size, std::ptrdiff_t offset) {
	return m_impl->map(size, offset);
}

/////////////////////////////////////
/////////////////////////////////////
void IndexBuffer::unmap() { m_impl->unmap(); }
