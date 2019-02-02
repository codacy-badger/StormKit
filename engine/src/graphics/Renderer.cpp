// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/graphics/Renderer.hpp>
#include <storm/engine/graphics/Drawable.hpp>

#include <storm/engine/render/Device.hpp>
#include <storm/engine/render/Surface.hpp>

#include <storm/core/TypeTraits.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
Renderer::Renderer(Device &device, Surface &surface, uvec2 internal_extent)
	: m_device{device}, 
	  m_surface{surface},
	  m_render_finished_fence{device},
	  m_semaphore{device},
	  m_forward_render_pass{device},
	  m_internal_extent{internal_extent},
	  m_matrices_uniform_buffer{device, sizeof(Matrices), alignof(Matrices)},
	  m_matrices_is_updated{true} {
	
	m_matrices.projection = mat4{1.f};
	m_matrices.view       = mat4{1.f};

	m_forward_render_pass.addAttachment(engine::ColorFormat::RGBA8888UNORM);
	m_forward_render_pass.addAttachment(device.bestDepthFormat());
	m_forward_render_pass.setExtent({m_internal_extent.x, m_internal_extent.y});
	
	auto subpass = RenderPass::SubPass{
		{RenderPass::SubPass::EXTERNAL},
		{},
		{0}
	};
	
	subpass.output_attachments.emplace_back(1);
	
	m_forward_render_pass.addSubPass(std::move(subpass));

	m_base_binding_state.bindings.emplace_back(
				engine::UniformBufferBinding{
					&m_matrices_uniform_buffer,
					0,
					sizeof(Matrices),
					0,
					engine::Shader::Stage::VERTEX
				}
			);
}

/////////////////////////////////////
/////////////////////////////////////
Renderer::~Renderer() = default;

/////////////////////////////////////
/////////////////////////////////////
Renderer::Renderer(Renderer &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void Renderer::prepareRender() {
	if(m_matrices_is_updated)
		uploadMatrices();

	m_render_finished_fence.wait();
	m_render_finished_fence.reset();
}

/////////////////////////////////////
/////////////////////////////////////
void Renderer::renderDrawable(const Drawable &drawable) {
	auto &command_buffer = buildOrAcquireCompatibleCommandBuffer(drawable);

	command_buffer.submit({}, { &m_semaphore });
}

/////////////////////////////////////
/////////////////////////////////////
void Renderer::render(const DrawableCache &cache) {
	for(const auto &drawable : cache.drawables())
		renderDrawable(drawable);
}

/////////////////////////////////////
/////////////////////////////////////
void Renderer::presentFrame() {
	m_surface.presentFrame(m_forward_render_pass.framebuffer(), m_semaphore, m_render_finished_fence);
}

/////////////////////////////////////
/////////////////////////////////////
CommandBuffer &Renderer::buildOrAcquireCompatibleCommandBuffer(const Drawable &drawable) {
	auto key = RendererCommandBufferCacheKey{};

	std::visit(
		core::overload {
			[&key](const VertexArray<Vertex_P> &vertices)
			{ key.vertex_count = std::size(vertices); },
			[&key](const VertexArray<Vertex_P_C> &vertices)
			{ key.vertex_count = std::size(vertices); },
			[&key](const VertexArray<Vertex_P_C_N> &vertices)
			{ key.vertex_count = std::size(vertices); },
			[&key](const VertexArray<Vertex_P_C_UV> &vertices)
			{ key.vertex_count = std::size(vertices); },
		},
		drawable.geometry.vertices
	);
	
	std::visit(
		core::overload {
			[&key](const IndexArray &indices)
			{ key.index_count = std::size(indices); },
			[&key](const LargeIndexArray &indices)
			{ key.index_count = std::size(indices); }
		},
		drawable.geometry.indices
	);
	
	key.state = drawable.pipeline_state;
	
	auto it = m_command_buffer_table.find(key);
	
	if(it != std::end(m_command_buffer_table)) {
		auto [_, buffer] = *it;
		
		return buffer.get();
	}

	VertexBuffer* vertex_buffer = nullptr;
	std::visit(
		core::overload {
			[this, &drawable, &vertex_buffer](const VertexArray<Vertex_P> &vertices) {
				vertex_buffer = &(m_vertex_buffers.emplace(drawable, m_device.createVertexBuffer(std::size(vertices) * sizeof(Vertex_P))).first->second);
				vertex_buffer->updateData(vertices, 0);
			},
			[this, &drawable, &vertex_buffer](const VertexArray<Vertex_P_C> &vertices) {
				vertex_buffer = &(m_vertex_buffers.emplace(drawable, m_device.createVertexBuffer(std::size(vertices) * sizeof(Vertex_P_C))).first->second);
				vertex_buffer->updateData(vertices, 0);
			},
			[this, &drawable, &vertex_buffer](const VertexArray<Vertex_P_C_N> &vertices) {
				vertex_buffer = &(m_vertex_buffers.emplace(drawable, m_device.createVertexBuffer(std::size(vertices) * sizeof(Vertex_P_C_N))).first->second);
				vertex_buffer->updateData(vertices, 0);
			},
			[this, &drawable, &vertex_buffer](const VertexArray<Vertex_P_C_UV> &vertices) {
				vertex_buffer = &(m_vertex_buffers.emplace(drawable, m_device.createVertexBuffer(std::size(vertices) * sizeof(Vertex_P_C_UV))).first->second);
				vertex_buffer->updateData(vertices, 0);
			}
		},
		drawable.geometry.vertices
	);

	IndexBuffer* index_buffer = nullptr;
	auto large = false;
	if(key.index_count) {
		std::visit(
			core::overload {
				[this, &drawable, &index_buffer, &large](const IndexArray &indices) {
					index_buffer = &(m_index_buffers.emplace(drawable, m_device.createIndexBuffer(std::size(indices) * sizeof(std::uint16_t))).first->second);
					index_buffer->updateData(indices, 0);
				},
				[this, &drawable, &index_buffer, &large](const LargeIndexArray &indices) {
					index_buffer = &(m_index_buffers.emplace(drawable, m_device.createIndexBuffer(std::size(indices) * sizeof(std::uint32_t))).first->second); large = true;
					index_buffer->updateData(indices, 0);
				},
			},
			drawable.geometry.indices
		);
	}

	auto &command_buffer = m_command_buffers.emplace_back(m_device);
	m_command_buffer_table.emplace(key, std::ref(command_buffer));

	command_buffer.pipelineState() = drawable.pipeline_state;
	command_buffer.bindingState() = drawable.binding_state;
				
	command_buffer.begin();
	command_buffer.beginRenderPass(m_forward_render_pass);
	command_buffer.setProgram(*drawable.program);

	command_buffer.bindVertexBuffer(0, *vertex_buffer);
	if(key.index_count > 0) {
		command_buffer.bindIndexBuffer(*index_buffer, large);
		command_buffer.drawIndexed(key.index_count, key.instance_count);
	} else
		command_buffer.draw(key.vertex_count, key.instance_count);

	command_buffer.endRenderPass();
	command_buffer.end();

	return command_buffer;
}

void Renderer::uploadMatrices() {
	m_matrices_uniform_buffer.updateData(reinterpret_cast<std::byte*>(&m_matrices), sizeof(Matrices), 0);

	m_matrices_is_updated = false;
}
