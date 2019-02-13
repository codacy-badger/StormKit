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
TextureImpl::TextureImpl(const Device &device, const image::Image &image)
    : m_image {image}, m_device {device} {
	if (m_image.channels() < 4) {
		auto missing_channels = 4 - m_image.channels();
		m_image.addChannels(missing_channels);
	}

	m_description = Texture::Description {1u, Format::RGBA8888UNORM,
	    {m_image.size().width, m_image.size().height, 1u}};

	initialise();
}

/////////////////////////////////////
/////////////////////////////////////
TextureImpl::TextureImpl(const Device &device, Texture::Description description)
    : m_description {std::move(description)}, m_image {}, m_device {device} {
	m_image.create(m_description.size.x, m_description.size.y, 4);

	initialise();
}

/////////////////////////////////////
/////////////////////////////////////
void TextureImpl::initialise() {
	auto image_create_info = vk::ImageCreateInfo {}
	                             .setImageType(vk::ImageType::e2D)
	                             .setFormat(asVK(m_description.format))
	                             .setExtent(vk::Extent3D {m_description.size.x,
	                                 m_description.size.y, 1u})
	                             .setArrayLayers(1)
	                             .setMipLevels(m_description.mip_level)
	                             .setSamples(vk::SampleCountFlagBits::e1)
	                             .setTiling(vk::ImageTiling::eOptimal)
	                             .setUsage(vk::ImageUsageFlagBits::eTransferDst
	                                       | vk::ImageUsageFlagBits::eSampled)
	                             .setSharingMode(vk::SharingMode::eExclusive)
	                             .setInitialLayout(vk::ImageLayout::eUndefined);

	auto sampler_create_info
	    = vk::SamplerCreateInfo {}
	          .setMagFilter(vk::Filter::eLinear)
	          .setMinFilter(vk::Filter::eLinear)
	          .setMipmapMode(vk::SamplerMipmapMode::eLinear)
	          .setAddressModeU(vk::SamplerAddressMode::eClampToEdge)
	          .setAddressModeV(vk::SamplerAddressMode::eClampToEdge);

	m_texture = m_device.implementation().createBackedVkTexture(
	    sampler_create_info, image_create_info, vk::ImageViewType::e2D,
	    vk::ImageAspectFlagBits::eColor, vk::ImageLayout::eTransferDstOptimal,
	    vk::MemoryPropertyFlagBits::eDeviceLocal);

	auto buffer = m_device.implementation().createBackedVkBuffer(
	    vk::BufferUsageFlagBits::eTransferSrc,
	    vk::MemoryPropertyFlagBits::eHostCoherent
	        | vk::MemoryPropertyFlagBits::eHostVisible,
	    m_image.byteNumbers());

	const auto &vk_device = m_device.implementation().vkDevice();
	auto        data      = reinterpret_cast<std::byte *>(
        vk_device.mapMemory(buffer.memory.get(), 0, m_image.byteNumbers()));

	std::memcpy(data, m_image.data(), m_image.byteNumbers());

	vk_device.unmapMemory(buffer.memory.get());

	auto cmd = m_device.createCommandBuffer();
	cmd.begin(true);
	cmd.implementation().transitionImageLayout(m_texture.image.image.get(),
	    vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal,
	    vk::PipelineStageFlagBits::eTopOfPipe,
	    vk::PipelineStageFlagBits::eTransfer);
	cmd.implementation().copyBufferToImage(buffer.buffer.get(),
	    m_texture.image.image.get(), m_texture.image.extent.x,
	    m_texture.image.extent.y);
	cmd.end();
	cmd.submit({}, {});

	m_device.waitIdle();
}

/////////////////////////////////////
/////////////////////////////////////
TextureImpl::~TextureImpl() = default;

/////////////////////////////////////
/////////////////////////////////////
TextureImpl::TextureImpl(TextureImpl &&) = default;
