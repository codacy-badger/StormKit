// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/graphics/ForwardRenderer.hpp>

#include <storm/core/Numerics.hpp>
#include <storm/core/Strings.hpp>

using namespace storm::engine;

struct PassData {
	std::vector<TextureResource *>diffuse_textures;

	TextureResource *backbuffer;
};

/////////////////////////////////////
/////////////////////////////////////
Renderer<RenderMethod::FORWARD_RENDERING>::Renderer(const Device &device, uvec2 swapchain_size, RenderGraph &graph)
	: m_device{device},
	  m_graph{graph},
	  m_backbuffer_desc{1,
						engine::Format::RGBA8888UNORM,
						engine::vec3{swapchain_size, 1u}
						},
	  m_backbuffer{m_device, m_backbuffer_desc} {

	m_backbuffer_resource = m_graph.get().addRetainedResource(
										"backbuffer",
										m_backbuffer_desc,
										&m_backbuffer);
};

/////////////////////////////////////
/////////////////////////////////////
Renderer<RenderMethod::FORWARD_RENDERING>::~Renderer() = default;

/////////////////////////////////////
/////////////////////////////////////
Renderer<RenderMethod::FORWARD_RENDERING>::Renderer(Renderer &&) = default;

/////////////////////////////////////
/////////////////////////////////////
Renderer<RenderMethod::FORWARD_RENDERING> &Renderer<RenderMethod::FORWARD_RENDERING>::operator=(Renderer &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void Renderer<RenderMethod::FORWARD_RENDERING>::addMesh(Mesh mesh) {
	m_mesh.emplace_back(mesh);

	auto &mesh_ref = m_mesh.back();

	if(mesh_ref.diffuse)
		appendTexture(mesh_ref.diffuse);
}

/////////////////////////////////////
/////////////////////////////////////
void Renderer<RenderMethod::FORWARD_RENDERING>::renderFrame() {

}

/////////////////////////////////////
/////////////////////////////////////
void Renderer<RenderMethod::FORWARD_RENDERING>::appendTexture(Texture *src) {
	auto texture_description = TextureDescription{};
	texture_description.size = {src->image().size().width, src->image().size().height, 1u};
	texture_description.format = engine::Format::RGBA8888UNORM;
	texture_description.mip_level = 1;

	auto size = std::size(m_texture_resources);
	m_texture_resources.emplace_back(m_graph.get().addRetainedResource(
									 "texture_" + storm::core::toString(size),
									 texture_description,
									 src));
}

/////////////////////////////////////
/////////////////////////////////////
void Renderer<RenderMethod::FORWARD_RENDERING>::updateUVs(Mesh &mesh) {

}

void Renderer<RenderMethod::FORWARD_RENDERING>::updateRenderGraph() {
	m_graph.get().removeRenderPass("forward_color_pass");

	m_forward_color_pass = &m_graph.get().addRenderPass<PassData>("forward_color_pass",
		[&](PassData &data, engine::RenderTaskBuilder &builder) {
			for(auto &texture_resource : m_texture_resources)
				data.diffuse_textures.emplace_back(builder.read<TextureResource>(texture_resource));

			data.backbuffer  = builder.write<TextureResource>(m_backbuffer_resource);
		},
		[=](const PassData &data) {

		}
	);
}
