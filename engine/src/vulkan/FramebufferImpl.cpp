// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/vulkan/FramebufferImpl.hpp>
#include <storm/engine/vulkan/RenderPassImpl.hpp>
#include <storm/engine/vulkan/TypeConversions.hpp>
#include <storm/engine/vulkan/Utils.hpp>
#include <storm/log/LogOutput.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
FramebufferImpl::FramebufferImpl(
	const Device &device)
	: m_device {device.implementation()}
{ }


/////////////////////////////////////
/////////////////////////////////////
FramebufferImpl::~FramebufferImpl() = default;

/////////////////////////////////////
/////////////////////////////////////
FramebufferImpl::FramebufferImpl(FramebufferImpl &&) = default;

/////////////////////////////////////
/////////////////////////////////////
bool FramebufferImpl::hasDepthAttachment() const noexcept {
	auto has_depth_attachment = false;

	for(const auto &attachment : m_attachments) {
		if(isDepthFormat(attachment.format)) {
			has_depth_attachment = true;
			break;
		}
	}

	return has_depth_attachment;
}

/////////////////////////////////////
/////////////////////////////////////
void FramebufferImpl::build(const vk::RenderPass &render_pass) {
	m_backed_attachments.reserve(std::size(m_attachments));

	for (const auto &attachment : m_attachments)
		addAttachment(attachment.mip_level, attachment.format);

	auto image_views = std::vector<vk::ImageView> {};
	image_views.reserve(std::size(m_backed_attachments));

	for (const auto &attachment : m_backed_attachments)
		image_views.emplace_back(attachment.view.get());

	const auto framebuffer_create_info
		= vk::FramebufferCreateInfo{}
		  .setRenderPass(render_pass)
		  .setAttachmentCount(std::size(image_views))
		  .setPAttachments(std::data(image_views))
		  .setWidth(m_extent.x)
		  .setHeight(m_extent.y)
		  .setLayers(1);

	m_framebuffer
		= m_device.vkDevice().createFramebufferUnique(framebuffer_create_info);

	storm::DLOG("renderer (vulkan)"_module, "framebuffer at %{1} allocated",
			&m_framebuffer.get());
}

/////////////////////////////////////
/////////////////////////////////////
void FramebufferImpl::addAttachment(uint8_t mip_level, ColorFormat format) {
	auto aspect_flags
		= vk::ImageAspectFlags {vk::ImageAspectFlagBits::eColor};
	auto image_usage
		= vk::ImageUsageFlagBits::eColorAttachment
		  | vk::ImageUsageFlagBits::eTransferSrc; // TODO conditionally
												  // check if transfert
												  // src is needed
	auto image_layout = vk::ImageLayout::eColorAttachmentOptimal;

	if (isDepthOnlyFormat(asVK(format))) {
		aspect_flags = vk::ImageAspectFlagBits::eDepth;
		image_usage  = vk::ImageUsageFlagBits::eDepthStencilAttachment;
		image_layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
	} else if (isDepthStencilFormat(asVK(format))) {
		aspect_flags = vk::ImageAspectFlagBits::eDepth
					   | vk::ImageAspectFlagBits::eStencil;
		image_usage  = vk::ImageUsageFlagBits::eDepthStencilAttachment;
		image_layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
	}

	auto image_info
		= vk::ImageCreateInfo {}
			  .setImageType(vk::ImageType::e2D)
			  .setFormat(asVK(format))
			  .setExtent({m_extent.x, m_extent.y, m_extent.z})
			  .setMipLevels(mip_level)
			  .setArrayLayers(1)
			  .setSamples(vk::SampleCountFlagBits::e1)
			  .setTiling(vk::ImageTiling::eOptimal)
			  .setUsage(image_usage)
			  .setSharingMode(vk::SharingMode::eExclusive)
			  .setInitialLayout(vk::ImageLayout::eUndefined);

	auto image = m_device.createBackedVkImage(image_info,
		vk::ImageViewType::e2D, aspect_flags, image_layout,
		vk::MemoryPropertyFlagBits::eDeviceLocal);

	m_backed_attachments.emplace_back(std::move(image));
}
