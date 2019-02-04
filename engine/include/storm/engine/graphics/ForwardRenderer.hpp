// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/graphics/Renderer.hpp>
#include <storm/engine/graphics/RenderGraph.hpp>
#include <storm/engine/graphics/Mesh.hpp>

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
			void updateRenderGraph();

			void renderFrame();
		private:
			void appendTexture(Texture *src);
			void updateUVs(Mesh &mesh);

			void preRender();
			void render();
			void postRender();

			std::reference_wrapper<const Device> m_device;
			std::reference_wrapper<RenderGraph>  m_graph;

			TextureDescription m_backbuffer_desc;
			Texture            m_backbuffer;
			TextureResource   *m_backbuffer_resource;

			RenderTask<PassData> *m_forward_color_pass;

			std::vector<Mesh>            m_mesh;
			std::vector<TextureResource*> m_texture_resources;

			std::unordered_map<Mesh*, uvec2> m_texture_position_map;
	};

	using ForwardRenderer = Renderer<RenderMethod::FORWARD_RENDERING>;
}
