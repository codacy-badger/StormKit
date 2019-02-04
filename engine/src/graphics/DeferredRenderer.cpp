// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/graphics/DeferredRenderer.hpp>

#include <storm/core/Numerics.hpp>
#include <storm/core/Strings.hpp>

using namespace storm::engine;

struct GBufferPassData {
	std::vector<TextureResource *>diffuse_textures;

	TextureResource *position;
	TextureResource *albedo;
	TextureResource *normal;
	TextureResource *specular;
};

struct LightningPassData {
	TextureResource *position;
	TextureResource *albedo;
	TextureResource *normal;
	TextureResource *specular;

	TextureResource *backbuffer;
};

/////////////////////////////////////
/////////////////////////////////////
Renderer<RenderMethod::DEFERRED_RENDERING>::Renderer(const Device &device, uvec2 swapchain_size, RenderGraph *graph)
	: m_device{device},
	  m_graph{graph},
	  m_swapchain_size{swapchain_size},
	  m_backbuffer_desc{1,
						engine::Format::RGBA8888UNORM,
						engine::vec3{swapchain_size, 1u}
						},
	  m_backbuffer{m_device, m_backbuffer_desc},
	  m_mesh_added{false} {


};

/////////////////////////////////////
/////////////////////////////////////
Renderer<RenderMethod::DEFERRED_RENDERING>::~Renderer() = default;

/////////////////////////////////////
/////////////////////////////////////
Renderer<RenderMethod::DEFERRED_RENDERING>::Renderer(Renderer &&) = default;

/////////////////////////////////////
/////////////////////////////////////
Renderer<RenderMethod::DEFERRED_RENDERING> &Renderer<RenderMethod::DEFERRED_RENDERING>::operator=(Renderer &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void Renderer<RenderMethod::DEFERRED_RENDERING>::addMesh(Mesh mesh) {
	m_mesh_added = true;

	m_mesh.emplace_back(mesh);

	auto &mesh_ref = m_mesh.back();

	if(mesh_ref.diffuse)
		appendTexture(mesh_ref.diffuse);
}

/////////////////////////////////////
/////////////////////////////////////
void Renderer<RenderMethod::DEFERRED_RENDERING>::renderFrame() {

}

/////////////////////////////////////
/////////////////////////////////////
void Renderer<RenderMethod::DEFERRED_RENDERING>::appendTexture(Texture *src) {
	auto texture_description = TextureDescription{};
	texture_description.size = {src->image().size().width, src->image().size().height, 1u};
	texture_description.format = engine::Format::RGBA8888UNORM;
	texture_description.mip_level = 1;

	auto size = std::size(m_texture_resources);
	m_texture_resources.emplace_back(m_graph->addRetainedResource(
										 "texture_" + storm::core::toString(size),
										 texture_description,
										 src));
}

/////////////////////////////////////
/////////////////////////////////////
void Renderer<RenderMethod::DEFERRED_RENDERING>::updateRenderGraph() {
	m_graph->removeRenderPass("deferred_lightning_pass");
	m_graph->removeRenderPass("deferred_gbuffer_pass");

	m_gbuffer_pass = &m_graph->addRenderPass<GBufferPassData>("deferred_gbuffer_pass",
		[&](GBufferPassData &data, engine::RenderTaskBuilder &builder) {
			for(auto &texture_resource : m_texture_resources)
				data.diffuse_textures.emplace_back(builder.read<TextureResource>(texture_resource));

			auto texture_desc = TextureDescription {
				1,
				engine::Format::RGBA8888UNORM,
				engine::vec3{m_swapchain_size, 1u}
			};

			data.position  = builder.create<TextureResource>("position", texture_desc);
			data.albedo    = builder.create<TextureResource>("albedo",   texture_desc);
			data.normal    = builder.create<TextureResource>("normal",   texture_desc);
			data.specular  = builder.create<TextureResource>("specular", texture_desc);
		},
		[](const GBufferPassData &data) {

		}
	);

	auto &gbuffer_data = m_gbuffer_pass->data();
	auto *backbuffer_resource = m_graph->addRetainedResource(
										"backbuffer",
										m_backbuffer_desc,
										&m_backbuffer);

	m_lightning_pass = &m_graph->addRenderPass<LightningPassData>("deferred_lightning_pass",
		[&](LightningPassData &data, engine::RenderTaskBuilder &builder) {
			data.position  = builder.read<TextureResource>(gbuffer_data.position);
			data.albedo    = builder.read<TextureResource>(gbuffer_data.albedo);
			data.normal    = builder.read<TextureResource>(gbuffer_data.normal);
			data.specular  = builder.read<TextureResource>(gbuffer_data.specular);

			data.backbuffer = builder.write(backbuffer_resource);
		},
		[](const LightningPassData &data) {

		}
	);
}
