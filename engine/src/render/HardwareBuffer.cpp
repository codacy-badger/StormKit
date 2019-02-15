// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/render/HardwareBuffer.hpp>
#include <storm/engine/vulkan/HardwareBufferImpl.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
HardwareBuffer::HardwareBuffer(const Device &device, Description description)
	: m_impl{device, std::move(description)} {

}

/////////////////////////////////////
/////////////////////////////////////
HardwareBuffer::~HardwareBuffer() = default;

/////////////////////////////////////
/////////////////////////////////////
HardwareBuffer::HardwareBuffer(HardwareBuffer &&) = default;

/////////////////////////////////////
/////////////////////////////////////
HardwareBuffer &HardwareBuffer::operator=(HardwareBuffer &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void HardwareBuffer::reset() {
	m_impl->reset();
}

/////////////////////////////////////
/////////////////////////////////////
void HardwareBuffer::setData(const std::byte *data, std::size_t size, std::ptrdiff_t offset) {
	m_impl->setData(data, size, offset);
}

/////////////////////////////////////
/////////////////////////////////////
std::byte *HardwareBuffer::map() {
	return m_impl->map();
}

/////////////////////////////////////
/////////////////////////////////////
std::byte *HardwareBuffer::map(std::size_t size, std::ptrdiff_t offset) {
	return m_impl->map(size, offset);
}

/////////////////////////////////////
/////////////////////////////////////
void HardwareBuffer::unmap() {
	m_impl->unmap();
}

/////////////////////////////////////
/////////////////////////////////////
const HardwareBuffer::Description &HardwareBuffer::description() {
	return m_impl->description();
}
