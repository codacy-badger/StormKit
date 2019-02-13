// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/graphics/RenderGraph.hpp>
#include <storm/engine/graphics/RenderTaskBase.hpp>
#include <storm/engine/graphics/Resource.hpp>
#include <storm/engine/render/Framebuffer.hpp>
#include <storm/engine/render/RenderPass.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
RenderTaskBase::RenderTaskBase(std::string name, RenderGraph &parent)
    : m_name {std::move(name)}, m_parent {parent}, m_id {0u}, m_ref_count {0u},
      m_framebuffer {nullptr} {}

/////////////////////////////////////
/////////////////////////////////////
RenderTaskBase::~RenderTaskBase() = default;

/////////////////////////////////////
/////////////////////////////////////
RenderTaskBase::RenderTaskBase(RenderTaskBase &&) = default;

/////////////////////////////////////
/////////////////////////////////////
RenderTaskBase &RenderTaskBase::operator=(RenderTaskBase &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void RenderTaskBase::initRenderPass(ResourcePool &pool) {
	m_render_pass.reset(nullptr);

	m_render_pass = m_parent.get().device().createRenderPassPtr();

	auto depth  = std::optional<std::reference_wrapper<const ResourceBase>> {};
	auto colors = std::vector<std::reference_wrapper<const ResourceBase>> {};

	auto subpass
	    = RenderPass::SubPass {{RenderPass::SubPass::EXTERNAL}, {}, {}};

	auto i = 1;
	for (const auto &resource_id : m_create_attachments) {
		const auto &resource      = pool.getTexture(resource_id);
		const auto &resource_desc = resource.description();

		if (!isDepthFormat(resource_desc.format))
			colors.emplace_back(resource);
		m_render_pass->addAttachment(resource_desc.format);

		subpass.input_attachments.emplace_back(i++);
	}

	for (const auto &resource_id : m_read_attachments) {
		const auto &resource      = pool.getTexture(resource_id);
		const auto &resource_desc = resource.description();

		m_render_pass->addAttachment(resource_desc.format);

		subpass.input_attachments.emplace_back(i++);
	}

	for (const auto &resource_id : m_write_attachments) {
		const auto &resource      = pool.getTexture(resource_id);
		const auto &resource_desc = resource.description();

		if (isDepthFormat(resource_desc.format))
			depth = resource;
		else
			colors.emplace_back(resource);

		m_render_pass->addAttachment(resource_desc.format);

		subpass.output_attachments.emplace_back(i++);
	}

	m_render_pass->addSubPass(std::move(subpass));
	m_render_pass->setExtent(computeSize(depth, colors));
	m_render_pass->build();

	m_framebuffer = Framebuffer::makeUnique(
	    m_parent.get().device(), m_render_pass->implementation());
}

uvec2 RenderTaskBase::computeSize(
    const std::optional<std::reference_wrapper<const ResourceBase>> &depth,
    const std::vector<std::reference_wrapper<const ResourceBase>> &  colors) {
	auto size = uvec2 {};

	if (depth) {
		auto &depth_resource
		    = static_cast<const TextureResource &>(depth.value().get());

		size = {depth_resource.description().size.x,
		    depth_resource.description().size.y};
	} else {
		auto &color_resource
		    = static_cast<const TextureResource &>(colors[0].get());

		size = {color_resource.description().size.x,
		    color_resource.description().size.y};
	}

	return size;
}
