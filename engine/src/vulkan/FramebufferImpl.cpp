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
    : m_device{device}
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

    for(const auto &[type, attachment] : m_attachments) {
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
    m_attachments.reserve(std::size(m_attachments));

    auto i = 0u;
    for (const auto &[type, attachment] : m_attachments) {
        if(type == AttachmentType::OUTPUT)
            addAttachment(i, attachment);
        ++i;
    }

	auto image_views = std::vector<vk::ImageView> {};
    image_views.reserve(std::size(m_attachments));

    i = 0u;
    for (const auto &[type, attachment] : m_attachments) {
        if(type == AttachmentType::OUTPUT) {
            const auto &texture = m_output_textures.at(i);
            const auto image_view = texture.implementation().backedVkTexture().image.view.get();

            image_views.emplace_back(image_view);
        } else {
            const auto texture = m_input_textures.at(i);
            const auto image_view = texture->implementation().backedVkTexture().image.view.get();

            image_views.emplace_back(image_view);
        }

        ++i;
    }

	const auto framebuffer_create_info
		= vk::FramebufferCreateInfo{}
		  .setRenderPass(render_pass)
		  .setAttachmentCount(std::size(image_views))
		  .setPAttachments(std::data(image_views))
		  .setWidth(m_extent.x)
		  .setHeight(m_extent.y)
		  .setLayers(1);

	m_framebuffer
        = m_device.implementation().vkDevice().createFramebufferUnique(framebuffer_create_info);

	storm::DLOG("renderer (vulkan)"_module, "framebuffer at %{1} allocated",
			&m_framebuffer.get());
}

/////////////////////////////////////
/////////////////////////////////////
void FramebufferImpl::addAttachment(std::uint32_t id, const Framebuffer::Attachment &attachment) {
    auto aspect_flags = ImageAspectFlag::COLOR;
    auto image_usage =
      ImageUsageFlag::COLOR_ATTACHMENT |
      ImageUsageFlag::TRANSFERT_SRC; // TODO conditionally
												  // check if transfert
												  // src is needed
    auto image_layout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL;

    if (isDepthOnlyFormat(asVK(attachment.format))) {
        aspect_flags = ImageAspectFlag::DEPTH;
        image_usage  = ImageUsageFlag::DEPTH_STENCIL_ATTACHMENT;
        image_layout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    } else if (isDepthStencilFormat(asVK(attachment.format))) {
        aspect_flags =
          ImageAspectFlag::DEPTH |
          ImageAspectFlag::STENCIL;
        image_usage  = ImageUsageFlag::DEPTH_STENCIL_ATTACHMENT;
        image_layout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	}

    const auto texture_description = Texture::Description {
        attachment.mip_level,
        attachment.format,
        attachment.size,
        image_usage,
        aspect_flags,
        image_layout
    };

    m_output_textures.emplace(id, Texture{m_device, std::move(texture_description)});
}
