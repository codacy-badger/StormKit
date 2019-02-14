// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/core/TypeTraits.hpp>
#include <storm/engine/vulkan/CommandBufferImpl.hpp>
#include <storm/engine/vulkan/DeviceImpl.hpp>
#include <storm/engine/vulkan/FenceImpl.hpp>
#include <storm/engine/vulkan/IndexBufferImpl.hpp>
#include <storm/engine/vulkan/ProgramImpl.hpp>
#include <storm/engine/vulkan/RenderPassImpl.hpp>
#include <storm/engine/vulkan/SemaphoreImpl.hpp>
#include <storm/engine/vulkan/ShaderImpl.hpp>
#include <storm/engine/vulkan/TextureImpl.hpp>
#include <storm/engine/vulkan/TypeConversions.hpp>
#include <storm/engine/vulkan/UniformBufferImpl.hpp>
#include <storm/engine/vulkan/VertexBufferImpl.hpp>
#include <storm/log/LogOutput.hpp>

using namespace storm::engine;

static const auto access_map = std::unordered_map<vk::ImageLayout,
    vk::AccessFlags> {{vk::ImageLayout::eUndefined, vk::AccessFlags {}},
    {vk::ImageLayout::eGeneral,
        vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite},
    {vk::ImageLayout::eColorAttachmentOptimal,
        vk::AccessFlagBits::eColorAttachmentRead
            | vk::AccessFlagBits::eColorAttachmentWrite},
    {vk::ImageLayout::eDepthStencilReadOnlyOptimal,
        vk::AccessFlagBits::eDepthStencilAttachmentRead},
    {vk::ImageLayout::eDepthStencilAttachmentOptimal,
        vk::AccessFlagBits::eDepthStencilAttachmentRead
            | vk::AccessFlagBits::eDepthStencilAttachmentWrite},
    {vk::ImageLayout::eShaderReadOnlyOptimal, vk::AccessFlagBits::eShaderRead},
    {vk::ImageLayout::eTransferDstOptimal, vk::AccessFlagBits::eTransferWrite},
    {vk::ImageLayout::eTransferSrcOptimal, vk::AccessFlagBits::eTransferRead},
    {vk::ImageLayout::ePresentSrcKHR, vk::AccessFlagBits::eMemoryRead}};

/////////////////////////////////////
/////////////////////////////////////
CommandBufferImpl::CommandBufferImpl(const Device &device, bool primary)
    : m_current_render_pass {nullptr}, m_current_program {nullptr},
      m_device {device}, m_is_pipeline_state_dirty {true},
      m_is_binding_state_dirty {true}, m_is_primary_command_buffer {primary} {
	createCommandBuffer();
	createDescriptorPool();
}

/////////////////////////////////////
/////////////////////////////////////
CommandBufferImpl::~CommandBufferImpl() = default;

/////////////////////////////////////
/////////////////////////////////////
CommandBufferImpl::CommandBufferImpl(CommandBufferImpl &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void CommandBufferImpl::reset() {
	m_current_render_pass = nullptr;
	m_current_program     = nullptr;
	m_pipeline_state      = PipelineState {};
	m_binding_state       = BindingState {};

	m_descriptor_sets.clear();
	m_command_buffer->reset({});
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBufferImpl::begin(bool one_time_submit) const {
	if (one_time_submit)
		m_command_buffer->begin(
		    {vk::CommandBufferUsageFlagBits::eOneTimeSubmit});
	else
		m_command_buffer->begin(
		    {vk::CommandBufferUsageFlagBits::eSimultaneousUse});
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBufferImpl::end() const { m_command_buffer->end(); }

/////////////////////////////////////
/////////////////////////////////////
void CommandBufferImpl::beginRenderPass(RenderPass &render_pass,
    const Framebuffer &buffer, core::RGBColorF clear_color) {
	m_current_render_pass = &render_pass;

	auto &render_pass_impl = render_pass.implementation();

	const auto &vk_render_pass = render_pass_impl.vkRenderPass();
	const auto &vk_framebuffer = buffer.implementation().vkFramebuffer();

	const auto clear_color_value = vk::ClearColorValue {}.setFloat32(
	    {clear_color.r, clear_color.g, clear_color.b, clear_color.a});

	auto clear_values = std::vector<vk::ClearValue> {};
	clear_values.emplace_back(clear_color_value);

	if (buffer.hasDepthAttachment()) {
		const auto depth_color_value
		    = vk::ClearDepthStencilValue {}.setDepth(1.f).setStencil(0.f);

		clear_values.emplace_back(depth_color_value);
	}

	const auto &extent = buffer.extent();
	const auto  render_pass_info
	    = vk::RenderPassBeginInfo {}
	          .setRenderPass(vk_render_pass)
	          .setFramebuffer(vk_framebuffer)
	          .setRenderArea(vk::Rect2D {{0, 0}, {extent.x, extent.y}})
	          .setClearValueCount(std::size(clear_values))
	          .setPClearValues(std::data(clear_values));

	m_command_buffer->beginRenderPass(
	    render_pass_info, vk::SubpassContents::eInline);
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBufferImpl::endRenderPass() {
	m_current_render_pass = nullptr;
	m_command_buffer->endRenderPass();
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBufferImpl::draw(std::size_t vertex_count,
    std::size_t instance_count, std::uint32_t first_vertex,
    std::uint32_t first_instance) {
	bindDescriptors();

	m_command_buffer->draw(
	    vertex_count, instance_count, first_vertex, first_instance);
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBufferImpl::drawIndexed(std::size_t index_count,
    std::size_t instance_count, std::uint32_t first_index,
    std::int32_t vertex_offset, std::uint32_t first_instance) {
	bindDescriptors();

	m_command_buffer->drawIndexed(index_count, instance_count, first_index,
	    vertex_offset, first_instance);
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBufferImpl::submit(
    const std::vector<const Semaphore *> &wait_semaphores,
    const std::vector<const Semaphore *> &signal_semaphores,
    std::vector<PipelineStage> pipeline_states, const Fence *fence) {
	const auto &vk_device = m_device.implementation().vkDevice();

	auto render_finished_fence
	    = (fence) ? fence->implementation().vkFence() : vk::Fence {nullptr};

	auto wait_stages = std::vector<vk::PipelineStageFlags> {};
	for (const auto &pipeline_state : pipeline_states)
		wait_stages.emplace_back(asVK(pipeline_state));

	auto wait_vk_semaphores = std::vector<vk::Semaphore> {};
	wait_vk_semaphores.reserve(std::size(wait_semaphores));

	for (const auto &semaphore : wait_semaphores)
		if (semaphore)
			wait_vk_semaphores.emplace_back(
			    semaphore->implementation().vkSemaphore());

	auto signal_vk_semaphores = std::vector<vk::Semaphore> {};
	signal_vk_semaphores.reserve(std::size(signal_semaphores));

	for (const auto &semaphore : signal_semaphores)
		if (semaphore)
			signal_vk_semaphores.emplace_back(
			    semaphore->implementation().vkSemaphore());

	const auto submit_info
	    = vk::SubmitInfo {}
	          .setPWaitDstStageMask(std::data(wait_stages))
	          .setWaitSemaphoreCount(std::size(wait_vk_semaphores))
	          .setPWaitSemaphores(std::data(wait_vk_semaphores))
	          .setCommandBufferCount(1)
	          .setPCommandBuffers(&(m_command_buffer.get()))
	          .setSignalSemaphoreCount(std::size(signal_vk_semaphores))
	          .setPSignalSemaphores(std::data(signal_vk_semaphores));

	m_device.implementation().vkGraphicsQueue().submit(
	    submit_info, render_finished_fence);
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBufferImpl::setProgram(const Program &program) {
	m_current_program = &program;
	m_current_pipeline_layout
	    = &(m_current_program->implementation().vkPipelineLayout());
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBufferImpl::transitionImageLayout(vk::Image image,
    vk::ImageLayout old_layout, vk::ImageLayout new_layout,
    vk::PipelineStageFlagBits source, vk::PipelineStageFlagBits destination,
    vk::ImageSubresourceRange range) {
	auto src_access = access_map.find(old_layout);
	auto dst_access = access_map.find(new_layout);

	if (src_access == std::end(access_map)
	    || dst_access == std::end(access_map)) {
		throw std::runtime_error(
		    "Failed to transition image layout: Unsupported transition!");
	}

	const auto barrier = vk::ImageMemoryBarrier {}
	                         .setOldLayout(old_layout)
	                         .setNewLayout(new_layout)
	                         .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
	                         .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
	                         .setImage(image)
	                         .setSubresourceRange(range)
	                         .setSrcAccessMask(src_access->second)
	                         .setDstAccessMask(dst_access->second);

	m_command_buffer->pipelineBarrier(source, destination,
	    vk::DependencyFlagBits(), nullptr, nullptr, barrier);
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBufferImpl::blitImage(vk::Image source, vk::Image destination,
    const uvec2 &src_size, const uvec2 &destination_size,
    vk::Filter filter) const {
	blitImage(source, 0, destination, 0, src_size, destination_size, 1, filter);
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBufferImpl::blitImage(vk::Image source,
    uint32_t source_mipmap_level, vk::Image destination,
    uint32_t destination_mipmap_level, const uvec2 &src_size,
    const uvec2 &destination_size, uint32_t layer_count,
    vk::Filter filter) const {

	const auto source_sub_ressource
	    = vk::ImageSubresourceLayers {}
	          .setAspectMask(vk::ImageAspectFlagBits::eColor)
	          .setLayerCount(layer_count)
	          .setMipLevel(source_mipmap_level);

	const auto destination_sub_ressource
	    = vk::ImageSubresourceLayers {}
	          .setAspectMask(vk::ImageAspectFlagBits::eColor)
	          .setLayerCount(layer_count)
	          .setMipLevel(destination_mipmap_level);

	const auto info
	    = vk::ImageBlit {}
	          .setSrcSubresource(source_sub_ressource)
	          .setDstSubresource(destination_sub_ressource)
	          .setSrcOffsets({vk::Offset3D {0, 0, 0},
	              vk::Offset3D {static_cast<std::int32_t>(src_size.x),
	                  static_cast<std::int32_t>(src_size.y), 1}})
	          .setDstOffsets({vk::Offset3D {0, 0, 0},
	              vk::Offset3D {static_cast<std::int32_t>(destination_size.x),
	                  static_cast<std::int32_t>(destination_size.y), 1}});

	m_command_buffer->blitImage(source, vk::ImageLayout::eTransferSrcOptimal,
	    destination, vk::ImageLayout::eTransferDstOptimal, info, filter);
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBufferImpl::copyBuffer(
    const vk::Buffer &src, const vk::Buffer &dest, vk::DeviceSize size) const {
	const auto copy_info
	    = vk::BufferCopy {}.setSrcOffset(0).setDstOffset(0).setSize(size);

	m_command_buffer->copyBuffer(src, dest, {copy_info});
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBufferImpl::copyBufferToImage(const vk::Buffer &src,
    const vk::Image &dest, std::uint32_t width, std::uint32_t height) const {
	const auto image_subresource
	    = vk::ImageSubresourceLayers {}
	          .setBaseArrayLayer(0)
	          .setMipLevel(0)
	          .setLayerCount(1)
	          .setAspectMask(vk::ImageAspectFlagBits::eColor);

	const auto copy_info = vk::BufferImageCopy {}
	                           .setImageExtent({width, height, 1})
	                           .setImageOffset({0, 0, 0})
	                           .setBufferImageHeight(0)
	                           .setBufferOffset(0)
	                           .setBufferRowLength(0)
	                           .setImageSubresource(image_subresource);

	m_command_buffer->copyBufferToImage(
	    src, dest, vk::ImageLayout::eTransferDstOptimal, {copy_info});
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBufferImpl::bindVertexBuffer(
    std::uint32_t index, const VertexBuffer &buffer) {
	m_command_buffer->bindVertexBuffers(
	    index, {buffer.implementation().backedVkBuffer().buffer.get()}, {0});
}

void CommandBufferImpl::bindIndexBuffer(
    const IndexBuffer &buffer, bool large_indices) {
	const auto &vk_buffer
	    = buffer.implementation().backedVkBuffer().buffer.get();

	if (!large_indices)
		m_command_buffer->bindIndexBuffer(vk_buffer, 0, vk::IndexType::eUint16);
	else
		m_command_buffer->bindIndexBuffer(vk_buffer, 0, vk::IndexType::eUint32);
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBufferImpl::executeCommandBuffers(
    const std::vector<std::reference_wrapper<CommandBuffer>> &command_buffers) {
	auto vk_command_buffers = std::vector<vk::CommandBuffer> {};
	vk_command_buffers.reserve(std::size(command_buffers));

	for (const auto &command_buffer : command_buffers) {
		vk_command_buffers.emplace_back(
		    command_buffer.get().implementation().vkCommandBuffer());
	}

	m_command_buffer->executeCommands(
	    std::size(vk_command_buffers), std::data(vk_command_buffers));
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBufferImpl::createCommandBuffer() {
	const auto &device = m_device.implementation();

	m_command_buffer
	    = device.allocateCommandBuffer(DeviceImpl::QueueType::GRAPHICS,
	        (m_is_primary_command_buffer) ? vk::CommandBufferLevel::ePrimary
	                                      : vk::CommandBufferLevel::eSecondary);
}

void CommandBufferImpl::createDescriptorPool() {
	const auto &device = m_device.implementation();

	const auto &pool_sizes = {
	    vk::DescriptorPoolSize {vk::DescriptorType::eUniformBuffer, 64},
	    vk::DescriptorPoolSize {vk::DescriptorType::eCombinedImageSampler, 64}};

	const auto create_info
	    = vk::DescriptorPoolCreateInfo {}
	          .setPoolSizeCount(std::size(pool_sizes))
	          .setPPoolSizes(std::data(pool_sizes))
	          .setMaxSets(64)
	          .setFlags(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet);

	m_descriptor_pool
	    = device.vkDevice().createDescriptorPoolUnique(create_info);
}

/////////////////////////////////////
/////////////////////////////////////
void CommandBufferImpl::bindDescriptors() {
	auto pipeline = getOrCreatePipeline();

	auto bind_point = vk::PipelineBindPoint::eGraphics;

	m_command_buffer->bindPipeline(bind_point, pipeline);

	auto descriptor_sets = getOrCreateDescriptorSets();

	m_command_buffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
	    *m_current_pipeline_layout, 0, descriptor_sets, {});
}

/////////////////////////////////////
/////////////////////////////////////
const vk::Pipeline &CommandBufferImpl::getOrCreatePipeline() {
	const auto &pipeline_cache = m_device.implementation().pipelineCache();

	auto key        = PipelineCacheKey {};
	key.program     = m_current_program;
	key.render_pass = m_current_render_pass;
	key.subpass     = nullptr;

	if (pipeline_cache.has(key))
		return pipeline_cache.acquire(key);

	const auto &shaders = m_current_program->shaderModules();

	auto shaders_create_info
	    = std::vector<vk::PipelineShaderStageCreateInfo> {};
	shaders_create_info.reserve(std::size(shaders));

	for (const auto &shader : shaders) {
		const auto &shader_impl = shader->implementation();

		const auto create_info = vk::PipelineShaderStageCreateInfo {}
		                             .setStage(asVK(shader_impl.stage()))
		                             .setModule(shader_impl.vkShaderModule())
		                             .setPName("main");

		shaders_create_info.emplace_back(std::move(create_info));
	}

	auto vertex_input_bindings
	    = std::vector<vk::VertexInputBindingDescription> {};
	auto vertex_input_attributes
	    = std::vector<vk::VertexInputAttributeDescription> {};
	vertex_input_bindings.reserve(
	    std::size(m_pipeline_state.vertex_input_state.bindings));
	vertex_input_attributes.reserve(
	    std::size(m_pipeline_state.vertex_input_state.attributes));

	for (const auto &binding : m_pipeline_state.vertex_input_state.bindings)
		vertex_input_bindings.emplace_back(
		    binding.binding, binding.stride, asVK(binding.rate));

	for (const auto &attribute : m_pipeline_state.vertex_input_state.attributes)
		vertex_input_attributes.emplace_back(attribute.location,
		    attribute.binding, asVK(attribute.type), attribute.offset);

	const auto vertex_input_info
	    = vk::PipelineVertexInputStateCreateInfo {}
	          .setVertexBindingDescriptionCount(
	              std::size(vertex_input_bindings))
	          .setVertexAttributeDescriptionCount(
	              std::size(vertex_input_attributes))
	          .setPVertexBindingDescriptions(std::data(vertex_input_bindings))
	          .setPVertexAttributeDescriptions(
	              std::data(vertex_input_attributes));

	const auto vertex_input_assembly_info
	    = vk::PipelineInputAssemblyStateCreateInfo {}
	          .setTopology(asVK(m_pipeline_state.topology))
	          .setPrimitiveRestartEnable(false);

	const auto viewport = vk::Viewport {}
	                          .setX(m_pipeline_state.viewport.left)
	                          .setY(m_pipeline_state.viewport.top)
	                          .setWidth(m_pipeline_state.viewport.width)
	                          .setHeight(m_pipeline_state.viewport.height)
	                          .setMinDepth(m_pipeline_state.viewport.min_depth)
	                          .setMaxDepth(m_pipeline_state.viewport.max_depth);

	const auto scissor
	    = vk::Rect2D {}
	          .setOffset(
	              {m_pipeline_state.scissor.x, m_pipeline_state.scissor.y})
	          .setExtent(
	              {static_cast<std::uint32_t>(m_pipeline_state.scissor.z),
	                  static_cast<std::uint32_t>(m_pipeline_state.scissor.w)});

	const auto pipeline_viewport_info = vk::PipelineViewportStateCreateInfo {}
	                                        .setViewportCount(1)
	                                        .setPViewports(&viewport)
	                                        .setScissorCount(1)
	                                        .setPScissors(&scissor);

	const auto pipeline_rasterizer_info
	    = vk::PipelineRasterizationStateCreateInfo {}
	          .setDepthClampEnable(false)
	          .setRasterizerDiscardEnable(false)
	          .setPolygonMode(asVK(m_pipeline_state.polygon_mode))
	          .setLineWidth(m_pipeline_state.line_width)
	          .setCullMode(asVK(m_pipeline_state.cull_mode))
	          .setFrontFace(asVK(m_pipeline_state.front_face))
	          .setDepthBiasEnable(false);

	const auto pipeline_multisampling_info
	    = vk::PipelineMultisampleStateCreateInfo {}
	          .setSampleShadingEnable(false)
	          .setRasterizationSamples(vk::SampleCountFlagBits::e1);

	const auto pipeline_color_blending_info
	    = vk::PipelineColorBlendAttachmentState {}
	          .setColorWriteMask(asVK(m_pipeline_state.blend_write_color_mask))
	          .setBlendEnable(m_pipeline_state.is_blending_enabled);

	const auto pipeline_color_blending_state
	    = vk::PipelineColorBlendStateCreateInfo {}
	          .setLogicOpEnable(false)
	          .setAttachmentCount(1)
	          .setPAttachments(&pipeline_color_blending_info);

	const auto pipeline_depth_stencil_state
	    = vk::PipelineDepthStencilStateCreateInfo {}
	          .setDepthTestEnable(m_pipeline_state.is_depth_test_enabled)
	          .setDepthWriteEnable(m_pipeline_state.is_depth_test_enabled)
	          .setDepthCompareOp(asVK(m_pipeline_state.depth_compare_operation))
	          .setDepthBoundsTestEnable(false)
	          .setStencilTestEnable(m_pipeline_state.is_stencil_test_enabled);

	auto pipeline_create_info
	    = vk::GraphicsPipelineCreateInfo {}
	          .setStageCount(std::size(shaders_create_info))
	          .setPStages(std::data(shaders_create_info))
	          .setPVertexInputState(&vertex_input_info)
	          .setPInputAssemblyState(&vertex_input_assembly_info)
	          .setPViewportState(&pipeline_viewport_info)
	          .setPRasterizationState(&pipeline_rasterizer_info)
	          .setPMultisampleState(&pipeline_multisampling_info)
	          .setPColorBlendState(&pipeline_color_blending_state)
	          .setPDepthStencilState(&pipeline_depth_stencil_state)
	          .setLayout(*m_current_pipeline_layout)
	          .setRenderPass(
	              m_current_render_pass->implementation().vkRenderPass())
	          .setSubpass(0);

	const auto &vk_device = m_device.implementation().vkDevice();
	auto        pipeline
	    = vk_device.createGraphicsPipelineUnique(nullptr, pipeline_create_info);
	if (!pipeline)
		throw std::runtime_error("Failed to create & pipeline");

	storm::DLOG("Renderer (vulkan)"_module, "Pipeline allocated at %{1}",
	    &pipeline.get());

	const_cast<PipelineCache &>(pipeline_cache).add(key, std::move(pipeline));
	const auto &pipeline_ = pipeline_cache.acquire(key);

	return pipeline_;
}

std::vector<vk::DescriptorSet> CommandBufferImpl::getOrCreateDescriptorSets() {
	const auto &device = m_device.implementation();

	if (m_is_binding_state_dirty) {
		const auto &descriptor_set_layouts
		    = m_current_program->implementation().vkDescriptorSetLayouts();
		auto descriptor_set_alloc_info
		    = vk::DescriptorSetAllocateInfo {}
		          .setDescriptorPool(m_descriptor_pool.get())
		          .setDescriptorSetCount(std::size(descriptor_set_layouts))
		          .setPSetLayouts(std::data(descriptor_set_layouts));

		m_descriptor_sets = device.vkDevice().allocateDescriptorSetsUnique(
		    descriptor_set_alloc_info);

		for (const auto &set : m_descriptor_sets) {
			auto buffer_infos = std::vector<
			    std::pair<std::uint32_t, vk::DescriptorBufferInfo>> {};
			auto image_infos = std::vector<
			    std::pair<std::uint32_t, vk::DescriptorImageInfo>> {};
			auto write_infos = std::vector<vk::WriteDescriptorSet> {};

			for (const auto &binding : m_binding_state.bindings) {
				std::visit(
				    core::overload {
				        [&](const UniformBufferBinding &buffer_binding) {
					        const auto &backed_buffer
					            = buffer_binding.buffer->implementation()
					                  .backedVkBuffer();
					        const auto buffer_info_
					            = vk::DescriptorBufferInfo {}
					                  .setBuffer(backed_buffer.buffer.get())
					                  .setOffset(0)
					                  .setRange(buffer_binding.size);

					        buffer_infos.emplace_back(buffer_binding.binding,
					            std::move(buffer_info_));
				        },
				        [&](const TextureBinding &image_binding) {
					        const auto &backed_texture
					            = image_binding.texture->implementation()
					                  .backedVkTexture();
					        const auto image_info_
					            = vk::DescriptorImageInfo {}
					                  .setImageLayout(
					                      backed_texture.image.layout)
					                  .setImageView(
					                      backed_texture.image.view.get())
					                  .setSampler(backed_texture.sampler.get());

					        image_infos.emplace_back(
					            image_binding.binding, std::move(image_info_));
				        }},
				    binding);
			}

			for (const auto &[binding, buffer_info] : buffer_infos) {
				const auto descriptor_write
				    = vk::WriteDescriptorSet {}
				          .setDstSet(set.get())
				          .setDstBinding(binding)
				          .setDstArrayElement(0)
				          .setDescriptorType(vk::DescriptorType::eUniformBuffer)
				          .setDescriptorCount(1)
				          .setPBufferInfo(&buffer_info);

				write_infos.emplace_back(descriptor_write);
			}

			for (const auto &[binding, image_info] : image_infos) {
				const auto descriptor_write
				    = vk::WriteDescriptorSet {}
				          .setDstSet(set.get())
				          .setDstBinding(binding)
				          .setDstArrayElement(0)
				          .setDescriptorType(
				              vk::DescriptorType::eCombinedImageSampler)
				          .setDescriptorCount(1)
				          .setPImageInfo(&image_info);

				write_infos.emplace_back(std::move(descriptor_write));
			}

			device.vkDevice().updateDescriptorSets(write_infos, {});
		}

		m_is_binding_state_dirty = false;
	}

	auto descriptor_sets = std::vector<vk::DescriptorSet> {};
	descriptor_sets.reserve(m_descriptor_sets.size());

	for (const auto &descriptor_set : m_descriptor_sets)
		descriptor_sets.emplace_back(descriptor_set.get());

	return descriptor_sets;
}

/////////////////////////////////////
/////////////////////////////////////
bool PipelineCacheKey::operator==(const PipelineCacheKey &other) const
    noexcept {
	return std::hash<PipelineCacheKey>()(*this)
	       == std::hash<PipelineCacheKey>()(other);
}
