// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/graphics/Renderer.hpp>
#include <storm/engine/graphics/RenderGraph.hpp>
#include <storm/engine/graphics/Mesh.hpp>

struct GBufferPassData;
struct LightningPassData;
namespace storm::engine {
	template <>
	class Renderer<RenderMethod::DEFERRED_RENDERING> : public core::NonCopyable {
		public:
			Renderer(const Device &device, uvec2 swapchain_size, RenderGraph *graph);
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
			RenderGraph *m_graph;

			uvec2 m_swapchain_size;

			TextureDescription m_backbuffer_desc;
			Texture            m_backbuffer;

			RenderTask<GBufferPassData>   *m_gbuffer_pass;
			RenderTask<LightningPassData> *m_lightning_pass;

			bool                          m_mesh_added;
			std::vector<Mesh>             m_mesh;
			std::vector<TextureResource*> m_texture_resources;

			std::unordered_map<Mesh*, uvec2> m_texture_position_map;
	};

	using DeferredRenderer = Renderer<RenderMethod::DEFERRED_RENDERING>;
}
