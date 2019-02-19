// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/vulkan/DeviceImpl.hpp>
#include <storm/engine/vulkan/RenderPassImpl.hpp>
#include <storm/engine/vulkan/TypeConversions.hpp>
#include <storm/engine/vulkan/Utils.hpp>
#include <storm/log/LogOutput.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
RenderPassImpl::RenderPassImpl(const Device &device)
	: m_device {device}, m_framebuffer{nullptr}, m_is_built{false} {}

/////////////////////////////////////
/////////////////////////////////////
RenderPassImpl::~RenderPassImpl() = default;

/////////////////////////////////////
/////////////////////////////////////
RenderPassImpl::RenderPassImpl(RenderPassImpl &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void RenderPassImpl::build() {
	ASSERT(m_framebuffer != nullptr, "Framebuffer need to be set before call build");

	createRenderPass();

	m_framebuffer->implementation().build(*m_render_pass);

	m_is_built = true;
}

/////////////////////////////////////
/////////////////////////////////////
std::size_t RenderPassImpl::addSubPass(RenderPass::SubPass &&subpass) {
	m_subpasses.emplace_back(std::move(subpass));

	const auto id = std::size(m_subpasses) - 1ul;

	return id;
}

/////////////////////////////////////
/////////////////////////////////////
void RenderPassImpl::createRenderPass() {
	const auto &vk_device        = m_device.implementation().vkDevice();
	const auto &attachments = m_framebuffer->attachments();

	auto attachment_formats = std::vector<ColorFormat>{};
	attachment_formats.reserve(std::size(attachments));

	std::transform(
		std::cbegin(attachments),
		std::cend(attachments),
		std::back_inserter(attachment_formats),
        [](const auto &pair) { return pair.second.format; }
	);
	const auto attachment_count = std::size(attachment_formats);

	auto attachment_descriptions = std::vector<vk::AttachmentDescription> {};
	auto attachment_references   = std::vector<vk::AttachmentReference> {};
	attachment_descriptions.reserve(attachment_count);
	attachment_references.reserve(attachment_count);

	auto depth_stencil_attachment_index = -1;

	for (const auto &attachment : std::as_const(attachment_formats)) {
		auto final_layout     = vk::ImageLayout::eDepthStencilAttachmentOptimal;
		auto stencil_load_op  = vk::AttachmentLoadOp::eClear;
		auto stencil_store_op = vk::AttachmentStoreOp::eStore;

		if (isColorFormat(asVK(attachment))) {
			final_layout     = vk::ImageLayout::eColorAttachmentOptimal;
			stencil_load_op  = vk::AttachmentLoadOp::eDontCare;
			stencil_store_op = vk::AttachmentStoreOp::eDontCare;
		} else if (isDepthOnlyFormat(asVK(attachment))) {
			final_layout     = vk::ImageLayout::eDepthStencilAttachmentOptimal;
			stencil_load_op  = vk::AttachmentLoadOp::eDontCare;
			stencil_store_op = vk::AttachmentStoreOp::eDontCare;
		}

		const auto attachment_description
		    = vk::AttachmentDescription {}
		          .setFormat(asVK(attachment))
		          .setSamples(vk::SampleCountFlagBits::e1)
		          .setLoadOp(vk::AttachmentLoadOp::eClear)
		          .setStoreOp(vk::AttachmentStoreOp::eStore)
		          .setStencilLoadOp(stencil_load_op) // TODO handle stencil
		          .setStencilStoreOp(stencil_store_op)
		          .setInitialLayout(vk::ImageLayout::eUndefined)
		          .setFinalLayout(final_layout);

		const auto attachment_reference
		    = vk::AttachmentReference {}
		          .setLayout(attachment_description.finalLayout)
		          .setAttachment(std::size(attachment_descriptions));

		attachment_descriptions.emplace_back(std::move(attachment_description));
		attachment_references.emplace_back(std::move(attachment_reference));

		if (isDepthFormat(asVK(attachment)))
			depth_stencil_attachment_index
			    = std::size(attachment_descriptions) - 1;
	}

	auto subpass_descriptions = std::vector<vk::SubpassDescription> {};
	auto input_attachments
	    = std::vector<std::vector<vk::AttachmentReference>> {};
	auto output_attachments
	    = std::vector<std::vector<vk::AttachmentReference>> {};

	subpass_descriptions.reserve(std::size(m_subpasses));
	input_attachments.resize(std::size(m_subpasses));
	output_attachments.resize(std::size(m_subpasses));

	auto subpass_index = 0;
	for (const auto &subpass : m_subpasses) {
		auto &input_attachments_subpass  = input_attachments[subpass_index];
		auto &output_attachments_subpass = output_attachments[subpass_index];

		auto subpass_description = vk::SubpassDescription {};

		for (auto input_attachment : subpass.input_attachments)
			input_attachments_subpass.emplace_back(
			    attachment_references[input_attachment]);

		for (auto output_attachment : subpass.output_attachments) {
			if (output_attachment == depth_stencil_attachment_index)
				subpass_description.setPDepthStencilAttachment(
				    &attachment_references[output_attachment]);
			else
				output_attachments_subpass.emplace_back(
				    attachment_references[output_attachment]);
		}

		subpass_description
		    .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
		    .setInputAttachmentCount(std::size(input_attachments_subpass))
		    .setPInputAttachments(std::data(input_attachments_subpass))
		    .setColorAttachmentCount(std::size(output_attachments_subpass))
		    .setPColorAttachments(std::data(output_attachments_subpass));

		subpass_descriptions.emplace_back(std::move(subpass_description));

		++subpass_index;
	}

	const auto create_info
	    = vk::RenderPassCreateInfo {}
	          .setAttachmentCount(std::size(attachment_descriptions))
	          .setPAttachments(std::data(attachment_descriptions))
	          .setSubpassCount(std::size(subpass_descriptions))
	          .setPSubpasses(std::data(subpass_descriptions));
	m_render_pass = vk_device.createRenderPassUnique(create_info);

	if (!m_render_pass)
		throw std::runtime_error("Failed to create a render pass");

	storm::DLOG("Renderer (vulkan)"_module, "Render pass at %{1} allocated",
	    &m_render_pass.get());
}
