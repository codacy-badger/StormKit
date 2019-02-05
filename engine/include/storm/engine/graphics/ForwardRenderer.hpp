// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/graphics/Renderer.hpp>
#include <storm/engine/graphics/RenderGraph.hpp>
#include <storm/engine/graphics/Mesh.hpp>

#include <storm/image/Image.hpp>

#include <list>
#include <unordered_map>

struct PassData;
namespace storm::engine {
	template <>
	class Renderer<RenderMethod::FORWARD_RENDERING> : public core::NonCopyable {
		public:
			Renderer(const Device &device, uvec2 swapchain_size, RenderGraph &graph);
			~Renderer();

			Renderer(Renderer &&);
			Renderer &operator=(Renderer &&);

			inline RenderGraph &renderGraph() noexcept;
			inline const RenderGraph &renderGraph() const noexcept;

			void addMesh(Mesh mesh);
			void buildGraph();
			
			void renderFrame();
		private:
			void appendTexture(image::Image &src);
			void updateUVs(Mesh &mesh);
			
			void traverseGraph();

			void preRender();
			void render();
			void postRender();

			std::reference_wrapper<const Device> m_device;
			std::reference_wrapper<RenderGraph>  m_graph;

			TextureDescription m_backbuffer_desc;
			Texture            m_backbuffer;

			std::vector<Mesh>         m_meshs;
			std::vector<Texture::Ptr> m_textures;
			std::unordered_map<std::uint32_t, std::vector<std::uint32_t>> m_mesh_texture_map;
			
			std::vector<RenderPass> m_render_passes;
	};

	using ForwardRenderer = Renderer<RenderMethod::FORWARD_RENDERING>;
}
