// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/core/Numerics.hpp>
#include <storm/core/Strings.hpp>
#include <storm/engine/graphics/Renderer.hpp>

using namespace storm::engine;

struct SubmitPassData {
	ResourceBase::ID depthbuffer;
	ResourceBase::ID backbuffer;
};

struct ForwardColorPassData {
	std::vector<ResourceBase::ID> diffuse_textures;
	std::vector<ResourceBase::ID> transform_buffers;

	ResourceBase::ID camera_buffer;
	ResourceBase::ID depthbuffer;
	ResourceBase::ID backbuffer;
};

struct DefferedGBufferPassData {
	std::vector<ResourceBase::ID> diffuse_textures;
	std::vector<ResourceBase::ID> transform_buffers;

	ResourceBase::ID position;
	ResourceBase::ID albedo;
	ResourceBase::ID normal;
	ResourceBase::ID specular;

	ResourceBase::ID camera_buffer;
};

struct DeferredLightningPassData {
	ResourceBase::ID position;
	ResourceBase::ID albedo;
	ResourceBase::ID normal;
	ResourceBase::ID specular;

	ResourceBase::ID camera_buffer;
	ResourceBase::ID depthbuffer;
	ResourceBase::ID backbuffer;
};

Renderer::Renderer(const Device &device, uvec2 swapchain_size)
    : m_device {device}, m_swapchain_size {swapchain_size}, m_graph {m_device},
      m_command_buffer {m_device}, m_render_pass {m_device},
      m_camera_buffer_desc {sizeof(m_camera), alignof(m_camera)},
      m_camera_buffer {m_device, m_camera_buffer_desc} {
	m_base_pipeline_state.viewport
	    = engine::Viewport {0.f, 0.f, static_cast<float>(m_swapchain_size.x),
	        static_cast<float>(m_swapchain_size.y)};

	m_base_pipeline_state.scissor
	    = engine::vec4 {0, 0, static_cast<float>(m_swapchain_size.x),
	        static_cast<float>(m_swapchain_size.y)};

	m_base_pipeline_state.cull_mode             = engine::CullMode::NONE;
	m_base_pipeline_state.is_depth_test_enabled = true;

	m_render_pass.setExtent(swapchain_size);
}

/////////////////////////////////////
/////////////////////////////////////
Renderer::~Renderer() = default;

/////////////////////////////////////
/////////////////////////////////////
Renderer::Renderer(Renderer &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void Renderer::addMesh(RenderMethod method, Mesh mesh) {
	auto &meshs = m_meshs[method];

	if (mesh.diffuse)
		m_textures.emplace_back(
		    m_device.get().createTexturePtr(mesh.diffuse.value().get()));

	const auto transform_buffer_description = UniformBuffer::Description {
	    sizeof(mesh.transform), alignof(mesh.transform)};

	m_transforms.emplace_back(m_device.get().createUniformBufferPtr(
	    std::move(transform_buffer_description)));
	meshs.emplace_back(std::move(mesh));

	auto &meshes    = m_mesh_map[method];
	auto  mesh_data = MeshData {std::size(m_meshs) - 1u};

	if (mesh.diffuse)
		mesh_data.textures.emplace_back(std::size(m_textures) - 1u);

	mesh_data.transform = std::size(m_transforms) - 1u;

	meshes.emplace_back(std::move(mesh_data));
}

/////////////////////////////////////
/////////////////////////////////////
void Renderer::exportRenderGraph(const _std::filesystem::path &filepath) const {
	m_graph.exportGraphviz(filepath);
}

/////////////////////////////////////
/////////////////////////////////////
void Renderer::renderFrame() {
	m_command_buffer.reset();
	m_graph.execute(m_command_buffer);
}

/////////////////////////////////////
/////////////////////////////////////
void storm::engine::Renderer::update() {
	uploadMatrices();
	computeGraph();
}

/////////////////////////////////////
/////////////////////////////////////
void storm::engine::Renderer::computeGraph() {
	m_graph.clear();

	/*auto backbuffer_resource = m_graph.addRetainedAttachment(
	                                    "backbuffer",
	                                    m_backbuffer_desc,
	                                    m_backbuffer);*/

	auto camera_buffer_resource = m_graph.addRetainedUniformBuffer(
	    "camera", m_camera_buffer_desc, m_camera_buffer);

	auto texture_resources
	    = std::unordered_map<RenderMethod, ResourceVector> {};
	auto transform_resources
	    = std::unordered_map<RenderMethod, ResourceVector> {};

	for (const auto &[method, datas] : m_mesh_map) {
		auto texture_resources_   = ResourceVector {};
		auto transform_resources_ = ResourceVector {};

		for (const auto &mesh : datas) {
			const auto &mesh_ = getMesh(method, mesh.id);

			for (const auto texture_id : mesh.textures) {
				auto *texture = m_textures[texture_id].get();

				const auto size = std::size(texture_resources_);
				texture_resources_.emplace_back(m_graph.addRetainedAttachment(
				    mesh_.name + "_texture_" + storm::core::toString(size),
				    texture->description(), *texture));
			}

			auto *transform = m_transforms[mesh.transform].get();

			const auto size = std::size(transform_resources_);
			transform_resources_.emplace_back(m_graph.addRetainedUniformBuffer(
			    mesh_.name + "_transform_" + storm::core::toString(size),
			    transform->description(), *transform));
		}

		texture_resources.emplace(method, std::move(texture_resources_));
		transform_resources.emplace(method, std::move(transform_resources_));
	}

	const auto &begin_pass = m_graph.addRenderPass<BeginPassData>(
	    "begin_pass",
	    [&](BeginPassData &data, engine::RenderTaskBuilder &builder) {
		    const auto backbuffer_desc
		        = Texture::Description {1, engine::Format::RGBA8888UNORM,
		            engine::vec3 {m_swapchain_size, 1u}};

		    const auto depthbuffer_desc
		        = Texture::Description {1, m_device.get().bestDepthFormat(),
		            engine::vec3 {m_swapchain_size, 1u}};

		    data.backbuffer = builder.createAttachment(
		        "backbuffer", std::move(backbuffer_desc));
		    data.depthbuffer = builder.createAttachment(
		        "depthbuffer", std::move(depthbuffer_desc));
	    },
	    [=](CommandBuffer &command_buffer, const BeginPassData &data,
	        RenderPass &render_pass, Framebuffer &framebuffer) {
		    command_buffer.begin();
		    command_buffer.beginRenderPass(render_pass, framebuffer);
		    command_buffer.pipelineState() = basePipelineState();
	    });
	const auto &begin_pass_data = begin_pass.data();

	const auto &forward_renderer_color_pass
	    = m_graph.addRenderPass<ForwardColorPassData>(
	        "forward_renderer_color_pass",
	        [&](ForwardColorPassData &     data,
	            engine::RenderTaskBuilder &builder) {
		        const auto &texture_resources_
		            = texture_resources[Renderer::FORWARD_METHOD];
		        const auto &transform_resources_
		            = transform_resources[Renderer::FORWARD_METHOD];

		        for (auto &texture_resource : texture_resources_)
			        data.diffuse_textures.emplace_back(
			            builder.readAttachment(texture_resource));
		        for (auto &transform_resource : transform_resources_)
			        data.transform_buffers.emplace_back(
			            builder.readUniformBuffer(transform_resource));

		        data.camera_buffer
		            = builder.readUniformBuffer(camera_buffer_resource);
		        data.backbuffer
		            = builder.writeAttachment(begin_pass_data.backbuffer);
		        data.depthbuffer
		            = builder.writeAttachment(begin_pass_data.depthbuffer);
	        },
	        [=](CommandBuffer &command_buffer, const ForwardColorPassData &data,
	            RenderPass &render_pass, Framebuffer &framebuffer) {
		        command_buffer.beginRenderPass(render_pass, framebuffer);
		        command_buffer.pipelineState() = basePipelineState();
	        });

	auto &deferred_renderer_gbuffer_pass
	    = m_graph.addRenderPass<DefferedGBufferPassData>(
	        "deferred_renderer_gbuffer_pass",
	        [&](DefferedGBufferPassData &  data,
	            engine::RenderTaskBuilder &builder) {
		        const auto &texture_resources_
		            = texture_resources[Renderer::DEFERRED_METHOD];
		        const auto &transform_resources_
		            = transform_resources[Renderer::DEFERRED_METHOD];

		        for (auto &texture_resource : texture_resources_)
			        data.diffuse_textures.emplace_back(
			            builder.readAttachment(texture_resource));
		        for (auto &transform_resource : transform_resources_)
			        data.transform_buffers.emplace_back(
			            builder.readUniformBuffer(transform_resource));

		        const auto description = Texture::Description {
		            1u, engine::Format::RGBA8888UNORM, {m_swapchain_size, 1u}};

		        data.camera_buffer
		            = builder.readAttachment(camera_buffer_resource);

		        data.albedo = builder.createAttachment("albedo", description);
		        data.normal = builder.createAttachment("normal", description);
		        data.position
		            = builder.createAttachment("position", description);
		        data.specular
		            = builder.createAttachment("specular", description);
	        },
	        [=](CommandBuffer &                command_buffer,
	            const DefferedGBufferPassData &data, RenderPass &render_pass,
	            Framebuffer &framebuffer) {
		        command_buffer.beginRenderPass(render_pass, framebuffer);
		        command_buffer.pipelineState() = basePipelineState();
	        });

	const auto &deferred_renderer_gbuffer_pass_data
	    = deferred_renderer_gbuffer_pass.data();

	auto &deferred_renderer_lightning_pass
	    = m_graph.addRenderPass<DeferredLightningPassData>(
	        "deferred_renderer_lightning_pass",
	        [&](DeferredLightningPassData &data,
	            engine::RenderTaskBuilder &builder) {
		        data.albedo = builder.readAttachment(
		            deferred_renderer_gbuffer_pass_data.albedo);
		        data.normal = builder.readAttachment(
		            deferred_renderer_gbuffer_pass_data.normal);
		        data.position = builder.readAttachment(
		            deferred_renderer_gbuffer_pass_data.position);
		        data.specular = builder.readAttachment(
		            deferred_renderer_gbuffer_pass_data.specular);

		        data.backbuffer
		            = builder.writeAttachment(begin_pass_data.backbuffer);
		        data.depthbuffer
		            = builder.writeAttachment(begin_pass_data.depthbuffer);
	        },
	        [=](CommandBuffer &                  command_buffer,
	            const DeferredLightningPassData &data, RenderPass &render_pass,
	            Framebuffer &framebuffer) {
		        command_buffer.beginRenderPass(render_pass, framebuffer);
		        command_buffer.pipelineState() = basePipelineState();
	        });

	for (const auto &[method, graph_update_func] : m_graph_update_funcs) {
		const auto graph_update_parameters
		    = GraphUpdateParameters {*this, begin_pass_data,
		        m_mesh_map.at(method), texture_resources.at(method),
		        transform_resources.at(method), camera_buffer_resource};

		graph_update_func(m_graph, graph_update_parameters);
	}

	m_graph.addRenderPass<SubmitPassData>(
	    "submit_pass",
	    [&](SubmitPassData &data, engine::RenderTaskBuilder &builder) {
		    data.backbuffer
		        = builder.readAttachment(begin_pass_data.backbuffer);
		    data.depthbuffer
		        = builder.readAttachment(begin_pass_data.depthbuffer);
	    },
	    [=](CommandBuffer &command_buffer, const SubmitPassData &data,
	        RenderPass & render_pass,
	        Framebuffer &framebuffer) { command_buffer.end(); });

	m_graph.compile();
}

/////////////////////////////////////
/////////////////////////////////////
void Renderer::uploadMatrices() {
	if (!m_camera_dirty.projection && !m_camera_dirty.view)
		return;

	if (m_camera_dirty.projection && !m_camera_dirty.view) {
		m_camera_buffer.updateData(
		    reinterpret_cast<const std::byte *>(&m_camera.projection),
		    sizeof(m_camera.projection), 0u);
		return;
	} else if (!m_camera_dirty.projection && m_camera_dirty.view) {
		m_camera_buffer.updateData(
		    reinterpret_cast<const std::byte *>(&m_camera.view),
		    sizeof(m_camera.view), sizeof(m_camera.projection));
		return;
	}

	m_camera_buffer.updateData(
	    reinterpret_cast<const std::byte *>(&m_camera), sizeof(m_camera), 0u);
}
