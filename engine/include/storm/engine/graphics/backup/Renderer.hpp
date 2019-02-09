// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/NonCopyable.hpp>
#include <storm/core/Memory.hpp>
#include <storm/core/Hash.hpp>

#include <storm/image/Image.hpp>

#include <storm/engine/render/Framebuffer.hpp>

#include <storm/engine/graphics/Renderer.hpp>
#include <storm/engine/graphics/RenderGraph.hpp>
#include <storm/engine/graphics/Mesh.hpp>
#include <storm/engine/graphics/Hashes.hpp>

#include <vector>
#include <unordered_map>

namespace storm::engine {
	using RenderMethod = std::uint64_t;
	using MeshID = std::uint64_t;
	using ResourceVector = std::vector<ResourceBase::ID>;

	struct MeshToken {
		RenderMethod method;
		MeshID       id;
	};

	struct BeginPassData {
			ResourceBase::ID backbuffer;
			ResourceBase::ID depthbuffer;
	};

	constexpr inline RenderMethod operator"" _hash(const char *str, std::size_t sz) {
		return core::ComponentHash<RenderMethod>(str, sz);
	}

	class Renderer : public core::NonCopyable {
		public:
			struct MeshData {
				MeshID id;
				std::vector<uint64_t> textures;
				std::uint64_t         transform;
			};
			using MeshDatas = std::vector<MeshData>;

			struct GraphUpdateParameters {
				const Renderer &renderer;

				const BeginPassData &begin_data;

				const MeshDatas &meshs;

				const ResourceVector &texture_resources;
				const ResourceVector &transform_resources;

				ResourceBase::ID camera_buffer_resource;
			};

			using GraphUpdateFunction = std::function<void(RenderGraph &, const GraphUpdateParameters &)>;

			SUR_Object(Renderer)

			constexpr static const auto FORWARD_METHOD = "FORWARD_METHOD"_hash;
			constexpr static const auto DEFERRED_METHOD = "DEFERRED_METHOD"_hash;

			Renderer(const Device &device, uvec2 swapchain_size);
			~Renderer();

			Renderer(Renderer &&);

			void addMesh(RenderMethod, Mesh mesh);
			void update();

			void exportRenderGraph(const _std::filesystem::path &filepath) const;

			void renderFrame();

			inline void setProjectionMatrix(mat4 projection) { m_camera.projection = std::move(projection); m_camera_dirty.projection = true; }
			inline void setViewMatrix(mat4 view) { m_camera.view = std::move(view); m_camera_dirty.view = true; }

			inline const Mesh &getMesh(RenderMethod method, MeshID id) const noexcept { ASSERT(id < std::size(m_meshs.at(method)), "Out of bound"); return m_meshs.at(method)[id]; }
			inline const PipelineState &basePipelineState() const noexcept { return m_base_pipeline_state; }

			template <typename T>
			void addGraphUpdateFunction(RenderMethod method, T &&func);
		private:
			using MeshVector = std::vector<Mesh>;
			using MeshMap = std::unordered_map<RenderMethod, MeshVector>;

			using MeshDataMap = std::unordered_map<RenderMethod, MeshDatas>;

			void computeGraph();
			void uploadMatrices();

			std::reference_wrapper<const Device> m_device;
			uvec2                                m_swapchain_size;

			RenderGraph      m_graph;
			CommandBuffer    m_command_buffer;
			RenderPass       m_render_pass;
			Framebuffer::Ptr m_framebuffer;

			PipelineState m_base_pipeline_state;

			struct {
				mat4 projection;
				mat4 view;
			} m_camera;

			struct {
				bool projection;
				bool view;
			} m_camera_dirty;

			MeshMap m_meshs;

			UniformBuffer::Description m_camera_buffer_desc;
			UniformBuffer              m_camera_buffer;

			std::vector<Texture::Ptr>       m_textures;
			std::vector<UniformBuffer::Ptr> m_transforms;

			MeshDataMap m_mesh_map;

			std::unordered_map<RenderMethod, GraphUpdateFunction> m_graph_update_funcs;

			std::vector<RenderPass> m_render_passes;
	};
}

#include "Renderer.tpp"
