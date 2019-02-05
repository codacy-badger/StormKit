// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/graphics/ForwardRenderer.hpp>

#include <storm/core/Numerics.hpp>
#include <storm/core/Strings.hpp>

using namespace storm::engine;

struct PassData {
	std::vector<ResourceBase::ID> diffuse_textures;

	ResourceBase::ID backbuffer;
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
	if(mesh.diffuse)
		m_textures.emplace_back(m_device.get().createTexturePtr(mesh.diffuse.value().get()));
	
	m_meshs.emplace_back(std::move(mesh));
	
	m_mesh_texture_map[std::size(m_meshs) - 1].emplace_back(std::size(m_textures) - 1);
}

/////////////////////////////////////
/////////////////////////////////////
void Renderer<RenderMethod::FORWARD_RENDERING>::renderFrame() {

}

/////////////////////////////////////
/////////////////////////////////////
void Renderer<RenderMethod::FORWARD_RENDERING>::appendTexture(image::Image &src) {
}

/////////////////////////////////////
/////////////////////////////////////
void Renderer<RenderMethod::FORWARD_RENDERING>::updateUVs(Mesh &mesh) {

}

/////////////////////////////////////
/////////////////////////////////////
void storm::engine::Renderer<RenderMethod::FORWARD_RENDERING>::traverseGraph() {
	
	
}

/////////////////////////////////////
/////////////////////////////////////
void storm::engine::Renderer<RenderMethod::FORWARD_RENDERING>::buildGraph() {
	auto backbuffer_resource = m_graph.get().addRetainedResource(
										"backbuffer",
										m_backbuffer_desc,
										m_backbuffer);
	
	auto texture_resources = std::vector<ResourceBase::ID>{};
	for(auto &texture : m_textures) {
		auto texture_description = TextureDescription{};
		texture_description.size = {texture->image().size().width, texture->image().size().height, 1u};
		texture_description.format = engine::Format::RGBA8888UNORM;
		texture_description.mip_level = 1;
		
		const auto size = std::size(texture_resources);
		texture_resources.emplace_back(m_graph.get().addRetainedResource(
										 "texture_" + storm::core::toString(size),
										 texture_description,
										 *texture));
	}
	
	m_graph.get().addRenderPass<PassData>("forward_color_pass",
		[&](PassData &data, engine::RenderTaskBuilder &builder) {
			for(auto &texture_resource : texture_resources)
				data.diffuse_textures.emplace_back(builder.read<TextureResource>(texture_resource));

			data.backbuffer  = builder.write<TextureResource>(backbuffer_resource);
		},
		[=](const PassData &data) {
			
		}
	);
}
