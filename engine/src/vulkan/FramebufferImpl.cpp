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
    const Device &device, const RenderPass &render_pass)
    : m_device {device.implementation()},
      m_render_pass {render_pass.implementation()} {
	createFramebuffer(true);
}

/////////////////////////////////////
/////////////////////////////////////
FramebufferImpl::FramebufferImpl(
    const Device &device, const RenderPassImpl &render_pass)
    : m_device {device.implementation()},
      m_render_pass {render_pass} {
	createFramebuffer(true);
}

/////////////////////////////////////
/////////////////////////////////////
FramebufferImpl::~FramebufferImpl() = default;

/////////////////////////////////////
/////////////////////////////////////
FramebufferImpl::FramebufferImpl(FramebufferImpl &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void FramebufferImpl::createFramebuffer(bool build_from_render_pass) {
	const auto &extent = m_render_pass.extent();
	if (build_from_render_pass) {
		const auto attachments = m_render_pass.attachments();

		m_attachments.reserve(std::size(attachments));

		for (const auto &attachment : attachments) {
			auto aspect_flags
			    = vk::ImageAspectFlags {vk::ImageAspectFlagBits::eColor};
			auto image_usage
			    = vk::ImageUsageFlagBits::eColorAttachment
			      | vk::ImageUsageFlagBits::eTransferSrc; // TODO conditionally
			                                              // check if transfert
			                                              // src is needed
			auto image_layout = vk::ImageLayout::eColorAttachmentOptimal;

			if (isDepthOnlyFormat(asVK(attachment))) {
				aspect_flags = vk::ImageAspectFlagBits::eDepth;
				image_usage  = vk::ImageUsageFlagBits::eDepthStencilAttachment;
				image_layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
			} else if (isDepthStencilFormat(asVK(attachment))) {
				aspect_flags = vk::ImageAspectFlagBits::eDepth
				               | vk::ImageAspectFlagBits::eStencil;
				image_usage  = vk::ImageUsageFlagBits::eDepthStencilAttachment;
				image_layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
			}

			auto image_info
			    = vk::ImageCreateInfo {}
			          .setImageType(vk::ImageType::e2D)
			          .setFormat(asVK(attachment))
			          .setExtent(vk::Extent3D {extent.x, extent.y, 1u})
			          .setMipLevels(1)
			          .setArrayLayers(1)
			          .setSamples(vk::SampleCountFlagBits::e1)
			          .setTiling(vk::ImageTiling::eOptimal)
			          .setUsage(image_usage)
			          .setSharingMode(vk::SharingMode::eExclusive)
			          .setInitialLayout(vk::ImageLayout::eUndefined);

			auto image = m_device.createBackedVkImage(image_info,
			    vk::ImageViewType::e2D, aspect_flags, image_layout,
			    vk::MemoryPropertyFlagBits::eDeviceLocal);
			
			m_attachments.emplace_back(std::move(image));
		}
	}

	auto image_views = std::vector<vk::ImageView> {};
	image_views.reserve(std::size(m_attachments));

	for (const auto &attachment : m_attachments) {
		image_views.emplace_back(attachment.view.get());
	}

	const auto framebuffer_create_info
	    = vk::FramebufferCreateInfo {}
	          .setRenderPass(m_render_pass.vkRenderPass())
	          .setAttachmentCount(std::size(image_views))
	          .setPAttachments(std::data(image_views))
	          .setWidth(extent.x)
	          .setHeight(extent.y)
	          .setLayers(1);

	m_framebuffer
	    = m_device.vkDevice().createFramebufferUnique(framebuffer_create_info);

	storm::DLOG("Renderer (vulkan)"_module, "Framebuffer at %{1} allocated",
	    &m_framebuffer.get());
}
