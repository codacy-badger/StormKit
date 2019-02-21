// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/vulkan/CommandBufferImpl.hpp>
#include <storm/engine/vulkan/DeviceImpl.hpp>
#include <storm/engine/vulkan/TextureImpl.hpp>
#include <storm/engine/vulkan/TypeConversions.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
TextureImpl::TextureImpl(const Device &device, Texture::Description description)
      : m_description{std::move(description)}, m_device{device} {
    initialise(m_description);
}

/////////////////////////////////////
/////////////////////////////////////
TextureImpl::~TextureImpl() = default;

/////////////////////////////////////
/////////////////////////////////////
TextureImpl::TextureImpl(TextureImpl &&) = default;

void TextureImpl::setImage(storm::image::Image image) {
    if(image.channels() < 4) {
        auto missing_channels = 4 - image.channels();
        image.addChannels(missing_channels);
    }

    auto buffer = m_device.implementation().createBackedVkBuffer(
      vk::BufferUsageFlagBits::eTransferSrc,
      vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible,
      image.byteNumbers());

    const auto &vk_device = m_device.implementation().vkDevice();

    auto data = reinterpret_cast<std::byte *>(
      vk_device.mapMemory(buffer.memory.get(), 0, image.byteNumbers()));

    std::memcpy(data, image.data(), image.byteNumbers());

    vk_device.unmapMemory(buffer.memory.get());

    auto fence = Fence{m_device};
    fence.reset();
    auto cmd = m_device.createCommandBuffer();
    cmd.begin(true);
    cmd.implementation().transitionImageLayout(
      m_texture.image.image.get(),
      vk::ImageLayout::eUndefined,
      vk::ImageLayout::eTransferDstOptimal,
      vk::PipelineStageFlagBits::eTopOfPipe,
      vk::PipelineStageFlagBits::eTransfer);
    cmd.implementation().copyBufferToImage(
      buffer.buffer.get(),
      m_texture.image.image.get(),
      m_texture.image.extent.x,
      m_texture.image.extent.y);
    cmd.end();
    cmd.submit({}, {}, QueueType::TRANSFERT, {}, &fence);

    fence.wait();
}

/////////////////////////////////////
/////////////////////////////////////
void TextureImpl::initialise(Texture::Description description) {
    auto image_create_info =
      vk::ImageCreateInfo{}
        .setImageType(vk::ImageType::e2D)
        .setFormat(asVK(m_description.format))
        .setExtent(vk::Extent3D{description.size.x, description.size.y, description.size.z})
        .setArrayLayers(1)
        .setMipLevels(m_description.mip_level)
        .setSamples(vk::SampleCountFlagBits::e1)
        .setTiling(vk::ImageTiling::eOptimal)
        .setUsage(asVK(description.image_usage))
        .setSharingMode(vk::SharingMode::eExclusive)
        .setInitialLayout(vk::ImageLayout::eUndefined);

    auto sampler_create_info = vk::SamplerCreateInfo{}
                                 .setMagFilter(vk::Filter::eLinear)
                                 .setMinFilter(vk::Filter::eLinear)
                                 .setMipmapMode(vk::SamplerMipmapMode::eLinear)
                                 .setAddressModeU(vk::SamplerAddressMode::eClampToEdge)
                                 .setAddressModeV(vk::SamplerAddressMode::eClampToEdge);

    m_texture = m_device.implementation().createBackedVkTexture(
      sampler_create_info,
      image_create_info,
      vk::ImageViewType::e2D,
      asVK(description.image_aspect),
      asVK(description.image_layout),
      vk::MemoryPropertyFlagBits::eDeviceLocal);
}
