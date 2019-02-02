// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/render/CommandBuffer.hpp>

#include <storm/engine/vulkan/CommandBufferImpl.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
CommandBuffer::CommandBuffer(const Device &device, bool primary)
	: m_impl{device, primary} {

}

/////////////////////////////////////
/////////////////////////////////////
CommandBuffer::~CommandBuffer() = default;

/////////////////////////////////////
/////////////////////////////////////
CommandBuffer::CommandBuffer(CommandBuffer &&) = default;

/////////////////////////////////////
/////////////////////////////////////
CommandBuffer &CommandBuffer::operator=(CommandBuffer &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void CommandBuffer::reset() {
	m_impl->reset();
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBuffer::begin(bool one_time_submit) {
	m_impl->begin(one_time_submit);
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBuffer::end() {
	m_impl->end();
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBuffer::beginRenderPass(RenderPass &render_pass, core::RGBColorF clear_color) {
	m_impl->beginRenderPass(render_pass, clear_color);
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBuffer::endRenderPass() {
	m_impl->endRenderPass();
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBuffer::draw(std::size_t vertex_count, std::size_t instance_count, 
						 std::uint32_t first_vertex, std::uint32_t first_instance) {
	m_impl->draw(vertex_count, instance_count, first_vertex, first_instance);
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBuffer::drawIndexed(std::size_t index_count, std::size_t instance_count, 
								std::uint32_t first_index, std::int32_t vertex_offset, std::uint32_t first_instance) {
	m_impl->drawIndexed(index_count, instance_count, first_index, vertex_offset, first_instance);
}

void CommandBuffer::submit(const std::vector<const Semaphore*> &wait_semaphores, const std::vector<const Semaphore*> &signal_semaphores, std::vector<PipelineStage> pipeline_states, const Fence *fence) {
	m_impl->submit(wait_semaphores, signal_semaphores, std::move(pipeline_states), fence);
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBuffer::setProgram(const Program &program) {
	m_impl->setProgram(program);
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBuffer::bindVertexBuffer(uint32_t index, const VertexBuffer &buffer) {
	m_impl->bindVertexBuffer(index, buffer);
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBuffer::bindIndexBuffer(const IndexBuffer &buffer, bool large_indices) {
	m_impl->bindIndexBuffer(buffer, large_indices);
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBuffer::executeCommandBuffers(const std::vector<std::reference_wrapper<CommandBuffer>> &command_buffers) {
	m_impl->executeCommandBuffers(command_buffers);
}

/////////////////////////////////////
/////////////////////////////////////
PipelineState &CommandBuffer::pipelineState() noexcept {
	return m_impl->pipelineState();
}

/////////////////////////////////////
/////////////////////////////////////
const PipelineState &CommandBuffer::pipelineState() const noexcept {
	return m_impl->pipelineState();
}

/////////////////////////////////////
/////////////////////////////////////
BindingState &CommandBuffer::bindingState() noexcept {
	return m_impl->bindingState();
}

/////////////////////////////////////
/////////////////////////////////////
const BindingState &CommandBuffer::bindingState() const noexcept {
	return m_impl->bindingState();
}
