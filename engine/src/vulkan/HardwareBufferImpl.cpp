// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/vulkan/CommandBufferImpl.hpp>
#include <storm/engine/vulkan/HardwareBufferImpl.hpp>
#include <storm/engine/vulkan/TypeConversions.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
HardwareBufferImpl::HardwareBufferImpl(
  const Device &device,
  HardwareBuffer::Description description)
      : m_device{device}, m_description{std::move(description)} {
    m_buffer = m_device.implementation().createBackedVkBuffer(
      asVK(m_description.usage) | vk::BufferUsageFlagBits::eTransferDst,
      vk::MemoryPropertyFlagBits::eDeviceLocal,
      m_description.size);

    m_stagging_buffer = m_device.implementation().createBackedVkBuffer(
      vk::BufferUsageFlagBits::eTransferSrc,
      vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible,
      m_description.size);
}

/////////////////////////////////////
/////////////////////////////////////
HardwareBufferImpl::~HardwareBufferImpl() = default;

/////////////////////////////////////
/////////////////////////////////////
HardwareBufferImpl::HardwareBufferImpl(HardwareBufferImpl &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void HardwareBufferImpl::reset() {
}

/////////////////////////////////////
/////////////////////////////////////
void HardwareBufferImpl::setData(const std::byte *data, std::size_t size, std::ptrdiff_t offset) {
    ASSERT((offset + size) <= m_description.size, "Offset + size cannot be > to the buffer size");

    auto mem = map(size, offset);

    std::memcpy(mem, data, size);

    unmap();
}

/////////////////////////////////////
/////////////////////////////////////
std::byte *HardwareBufferImpl::map() {
    return map(m_description.size, 0u);
}

/////////////////////////////////////
/////////////////////////////////////
std::byte *HardwareBufferImpl::map(std::size_t size, std::ptrdiff_t offset) {
    m_mapped_data = reinterpret_cast<std::byte *>(
      m_device.implementation().vkDevice().mapMemory(m_stagging_buffer.memory.get(), offset, size));
    return m_mapped_data;
}

/////////////////////////////////////
/////////////////////////////////////
void HardwareBufferImpl::unmap() {
    m_mapped_data = nullptr;
    m_device.implementation().vkDevice().unmapMemory(m_stagging_buffer.memory.get());

    flush();
}

/////////////////////////////////////
/////////////////////////////////////
void HardwareBufferImpl::flush() {
    auto cmd = CommandBuffer{m_device};

    cmd.begin(true);
    cmd.implementation().copyBuffer(
      m_stagging_buffer.buffer.get(), m_buffer.buffer.get(), m_description.size);
    cmd.end();

    cmd.submit({}, {}, QueueType::TRANSFERT);
    m_device.waitIdle();
}
