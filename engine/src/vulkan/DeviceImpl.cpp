// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <set>
#include <storm/engine/vulkan/CommandBufferImpl.hpp>
#include <storm/engine/vulkan/ContextImpl.hpp>
#include <storm/engine/vulkan/DeviceImpl.hpp>
#include <storm/engine/vulkan/PhysicalDeviceImpl.hpp>
#include <storm/engine/vulkan/TypeConversions.hpp>
#include <storm/log/LogOutput.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
DeviceImpl::DeviceImpl(const Device &parent, const Context &context,
    window::NativeHandle handle, PhysicalDevice &&device)
    : m_parent {parent}, m_context {context}, m_physical_device {
                                                  std::move(device)} {
	updateQueueData(handle);
	createLogicalDevice();
	createCommandPools();
}

/////////////////////////////////////
/////////////////////////////////////
DeviceImpl::~DeviceImpl() = default;

/////////////////////////////////////
/////////////////////////////////////
DeviceImpl::DeviceImpl(DeviceImpl &&) = default;

/////////////////////////////////////
/////////////////////////////////////
ColorFormat DeviceImpl::bestDepthFormat() const noexcept {
	return fromVK(findSupportedFormat(
	    {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint,
	        vk::Format::eD24UnormS8Uint},
	    vk::ImageTiling::eOptimal,
					  vk::FormatFeatureFlagBits::eDepthStencilAttachment));
}

/////////////////////////////////////
/////////////////////////////////////
BackedSwapchainImage DeviceImpl::createBackedSwapchainImage(vk::Format format, vk::Image image) const {
	const auto components = vk::ComponentMapping {}
								.setR(vk::ComponentSwizzle::eIdentity)
								.setG(vk::ComponentSwizzle::eIdentity)
								.setB(vk::ComponentSwizzle::eIdentity)
								.setA(vk::ComponentSwizzle::eIdentity);

	const auto subressource_range
		= vk::ImageSubresourceRange {}
			  .setAspectMask(vk::ImageAspectFlagBits::eColor)
			  .setBaseMipLevel(0)
			  .setLevelCount(1)
			  .setBaseArrayLayer(0)
			  .setLayerCount(1);

	const auto create_info = vk::ImageViewCreateInfo {}
		.setImage(image)
		.setViewType(vk::ImageViewType::e2D)
		.setFormat(format)
		.setComponents(components)
		.setSubresourceRange(subressource_range);

	auto image_view = m_logical_device->createImageViewUnique(create_info);

	return BackedSwapchainImage{
		image,
		std::move(image_view)
	};
}

/////////////////////////////////////
/////////////////////////////////////
BackedVkImage DeviceImpl::createBackedVkImage(const vk::ImageCreateInfo &infos,
    vk::ImageViewType type, vk::ImageAspectFlags aspect, vk::ImageLayout layout,
    vk::MemoryPropertyFlags memory_properties) const {
	auto image = m_logical_device->createImageUnique(infos);

	storm::DLOG(
	    "Renderer (vulkan)"_module, "Image at %{1} allocated", &image.get());

	const auto memory_requirements
	    = m_logical_device->getImageMemoryRequirements(image.get());

	const auto image_memory_allocate_info
	    = vk::MemoryAllocateInfo {}
	          .setAllocationSize(memory_requirements.size)
	          .setMemoryTypeIndex(findMemoryRequirement(
	              memory_requirements.memoryTypeBits, memory_properties));

	auto device_memory
	    = m_logical_device->allocateMemoryUnique(image_memory_allocate_info);

	storm::DLOG("Renderer (vulkan)"_module, "Device Memory at %{1} allocated",
	    &device_memory.get());

	m_logical_device->bindImageMemory(image.get(), device_memory.get(), 0);

	const auto subresource_range = vk::ImageSubresourceRange {}
	                                   .setAspectMask(aspect)
	                                   .setBaseMipLevel(0u)
	                                   .setLevelCount(infos.mipLevels)
	                                   .setBaseArrayLayer(0u)
	                                   .setLayerCount(infos.arrayLayers);

	const auto image_view_create_info
	    = vk::ImageViewCreateInfo {}
	          .setImage(image.get())
	          .setViewType(type)
	          .setFormat(infos.format)
	          .setSubresourceRange(subresource_range);

	auto image_view
	    = m_logical_device->createImageViewUnique(image_view_create_info);

	storm::DLOG("Renderer (vulkan)"_module, "Image View at %{1} allocated",
	    &image_view.get());

	const auto sub_ressource_range = vk::ImageSubresourceRange {}
	                                     .setLevelCount(infos.mipLevels)
	                                     .setLayerCount(infos.arrayLayers)
	                                     .setAspectMask(aspect);

	const auto barrier = vk::ImageMemoryBarrier {}
	                         .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
	                         .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
	                         .setImage(image.get())
	                         .setOldLayout(infos.initialLayout)
	                         .setNewLayout(layout)
	                         .setSubresourceRange(sub_ressource_range);

	auto cmd = m_parent.createCommandBuffer();
	cmd.begin(true);
	cmd.implementation().vkCommandBuffer().pipelineBarrier(
	    vk::PipelineStageFlagBits::eTransfer,
	    vk::PipelineStageFlagBits::eTransfer, vk::DependencyFlagBits(), nullptr,
	    nullptr, barrier);
	cmd.end();

	auto submit_info
	    = vk::SubmitInfo {}.setCommandBufferCount(1).setPCommandBuffers(
	        &(cmd.implementation().vkCommandBuffer()));

	m_graphics_queue.submit(submit_info, nullptr);
	m_graphics_queue.waitIdle();

	return {std::move(image), std::move(image_view), std::move(device_memory),
	    {infos.extent.width, infos.extent.height}, layout};
}

/////////////////////////////////////
/////////////////////////////////////
BackedVkTexture DeviceImpl::createBackedVkTexture(
    const vk::SamplerCreateInfo &sampler_info, const vk::ImageCreateInfo &infos,
    vk::ImageViewType type, vk::ImageAspectFlags aspect, vk::ImageLayout layout,
    vk::MemoryPropertyFlags memory_properties) const {
	auto image
	    = createBackedVkImage(infos, type, aspect, layout, memory_properties);

	auto sampler = m_logical_device->createSamplerUnique(sampler_info);

	return {std::move(sampler), std::move(image)};
}

/////////////////////////////////////
/////////////////////////////////////
BackedVkBuffer DeviceImpl::createBackedVkBuffer(vk::BufferUsageFlags usage,
    vk::MemoryPropertyFlags memory_properties, vk::DeviceSize size) const {
	const auto buffer_info
	    = vk::BufferCreateInfo {}.setSize(size).setUsage(usage).setSharingMode(
	        vk::SharingMode::eExclusive);

	auto buffer = m_logical_device->createBufferUnique(buffer_info);

	storm::DLOG(
	    "Renderer (vulkan)"_module, "Buffer at %{1} allocated", &buffer.get());

	const auto memory_requirements
	    = m_logical_device->getBufferMemoryRequirements(buffer.get());

	const auto buffer_memory_allocate_info
	    = vk::MemoryAllocateInfo {}
	          .setAllocationSize(memory_requirements.size)
	          .setMemoryTypeIndex(findMemoryRequirement(
	              memory_requirements.memoryTypeBits, memory_properties));

	auto device_memory
	    = m_logical_device->allocateMemoryUnique(buffer_memory_allocate_info);

	storm::DLOG("Renderer (vulkan)"_module, "Device Memory at %{1} allocated",
	    &device_memory.get());

	m_logical_device->bindBufferMemory(buffer.get(), device_memory.get(), 0);

	return {std::move(buffer), std::move(device_memory)};
}

UniqueHandle<vk::CommandBuffer> DeviceImpl::allocateCommandBuffer(
    DeviceImpl::QueueType type, vk::CommandBufferLevel level) const {
	const auto command_pool_type
	    = static_cast<std::underlying_type_t<DeviceImpl::QueueType>>(type);

	const auto allocate_command_buffer_info
	    = vk::CommandBufferAllocateInfo {}
	          .setCommandPool(m_command_pools[command_pool_type].get())
	          .setLevel(level)
	          .setCommandBufferCount(1);

	auto command_buffers = m_logical_device->allocateCommandBuffersUnique(
	    allocate_command_buffer_info);

	storm::DLOG("Renderer (vulkan)"_module, "Command Buffer allocated at %{1}",
	    &command_buffers[0].get());

	return std::move(command_buffers[0]);
}

/////////////////////////////////////
/////////////////////////////////////
std::int32_t DeviceImpl::findMemoryRequirement(
    std::uint32_t filter, vk::MemoryPropertyFlags properties) const {
	const auto &vk_physical_device
	    = m_physical_device.implementation().vkPhysicalDevice();

	auto memory_properties = vk_physical_device.getMemoryProperties();

	for (auto i = 0; i < memory_properties.memoryTypeCount; ++i) {
		if ((filter & (1 << i))
		    && (memory_properties.memoryTypes[i].propertyFlags & properties)
		           == properties) {
			return i;
		}
	}

	return -1;
}

void DeviceImpl::waitIdle() const { m_logical_device->waitIdle(); }

/////////////////////////////////////
/////////////////////////////////////
void DeviceImpl::createLogicalDevice() {
	auto queue_priorities = 1.f;

	auto unique_queue_families = std::set<std::uint32_t> {};

	if (m_family_queue_indices.graphics_queue_index != 255)
		unique_queue_families.insert(
		    m_family_queue_indices.graphics_queue_index);

	if (m_family_queue_indices.compute_queue_index != 255)
		unique_queue_families.insert(
		    m_family_queue_indices.compute_queue_index);

	if (m_family_queue_indices.transfert_queue_index != 255)
		unique_queue_families.insert(
		    m_family_queue_indices.transfert_queue_index);

	std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
	queue_create_infos.reserve(std::size(unique_queue_families));
	for (const auto &queue_family : unique_queue_families) {
		auto device_queue_create_info
		    = vk::DeviceQueueCreateInfo {}
		          .setPQueuePriorities(&queue_priorities)
		          .setQueueFamilyIndex(queue_family)
		          .setQueueCount(1);

		queue_create_infos.emplace_back(std::move(device_queue_create_info));
	}

	auto device_create_info
	    = vk::DeviceCreateInfo {}
	          .setQueueCreateInfoCount(std::size(queue_create_infos))
	          .setPQueueCreateInfos(std::data(queue_create_infos))
	          .setEnabledExtensionCount(0);

	if (m_family_queue_indices.transfert_queue_index != 255) {
		device_create_info.setEnabledExtensionCount(1)
		    .setPpEnabledExtensionNames(
		        std::data(ContextImpl::DEVICE_EXTENSIONS));
	}

	const auto &vk_physical_device
	    = m_physical_device.implementation().vkPhysicalDevice();
	m_logical_device
	    = vk_physical_device.createDeviceUnique(device_create_info);
	if (!m_logical_device)
		throw std::runtime_error("Failed to create logical device");

	if (m_family_queue_indices.graphics_queue_index != 255)
		m_graphics_queue = m_logical_device->getQueue(
		    m_family_queue_indices.graphics_queue_index, 0);

	if (m_family_queue_indices.compute_queue_index != 255)
		m_compute_queue = m_logical_device->getQueue(
		    m_family_queue_indices.compute_queue_index, 0);

	if (m_family_queue_indices.transfert_queue_index != 255)
		m_transfert_queue = m_logical_device->getQueue(
		    m_family_queue_indices.transfert_queue_index, 0);

	storm::DLOG("Renderer (vulkan)"_module, "Logical Device allocated at %{1}",
	    &m_logical_device.get());
}

/////////////////////////////////////
/////////////////////////////////////
void DeviceImpl::updateQueueData(window::NativeHandle handle) {
	const auto &vk_physical_device
	    = m_physical_device.implementation().vkPhysicalDevice();

	const auto queue_families = vk_physical_device.getQueueFamilyProperties();

	if (std::size(queue_families) == 0u)
		throw std::runtime_error("This GPU doesn't have any queue families!");

	auto current_index = 0u;
	for (const auto &queue_family : std::as_const(queue_families)) {
		if (queue_family.queueCount > 0) {
			if (queue_family.queueFlags & vk::QueueFlagBits::eGraphics)
				m_family_queue_indices.graphics_queue_index = current_index;
			if (queue_family.queueFlags & vk::QueueFlagBits::eCompute)
				m_family_queue_indices.compute_queue_index = current_index;

#if defined(VK_USE_PLATFORM_XCB_KHR)
			struct Handles {
				xcb_connection_t *connection;
				xcb_window_t      window;
			} *handles = reinterpret_cast<Handles *>(handle);
			const auto visual_id
			    = xcb_setup_roots_iterator(xcb_get_setup(handles->connection))
			          .data->root_visual;
			const auto present_support
			    = vkGetPhysicalDeviceXcbPresentationSupportKHR(
			        vk_physical_device, current_index, handles->connection,
			        visual_id);
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
			const auto present_support = true;
#endif

			if (present_support)
				m_family_queue_indices.transfert_queue_index = current_index;

			++current_index;
		}
	}
}

/////////////////////////////////////
/////////////////////////////////////
void DeviceImpl::createCommandPools() {
	const auto graphics_command_pool_create_info
	    = vk::CommandPoolCreateInfo {}
	          .setQueueFamilyIndex(m_family_queue_indices.graphics_queue_index)
	          .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);

	m_command_pools[0] = m_logical_device->createCommandPoolUnique(
	    graphics_command_pool_create_info);
	storm::DLOG("Renderer (vulkan)"_module, "Command Pool allocated at %{1}",
	    &m_command_pools[0].get());

	const auto compute_command_pool_create_info
	    = vk::CommandPoolCreateInfo {}
	          .setQueueFamilyIndex(m_family_queue_indices.compute_queue_index)
	          .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);

	m_command_pools[1] = m_logical_device->createCommandPoolUnique(
	    compute_command_pool_create_info);
	storm::DLOG("Renderer (vulkan)"_module, "Command Pool allocated at %{1}",
	    &m_command_pools[1].get());

	const auto transfert_command_pool_create_info
	    = vk::CommandPoolCreateInfo {}
	          .setQueueFamilyIndex(m_family_queue_indices.transfert_queue_index)
	          .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);

	m_command_pools[2] = m_logical_device->createCommandPoolUnique(
	    transfert_command_pool_create_info);
	storm::DLOG("Renderer (vulkan)"_module, "Command Pool allocated at %{1}",
	    &m_command_pools[2].get());
}

/////////////////////////////////////
/////////////////////////////////////
vk::Format DeviceImpl::findSupportedFormat(
    const std::vector<vk::Format> &candidates, vk::ImageTiling tiling,
    vk::FormatFeatureFlags features) const noexcept {
	for (auto format : candidates) {
		auto properties = m_physical_device.implementation()
		                      .vkPhysicalDevice()
		                      .getFormatProperties(format);

		if ((tiling == vk::ImageTiling::eLinear
		        && (properties.linearTilingFeatures & features) == features)
		    || (tiling == vk::ImageTiling::eOptimal
		        && (properties.optimalTilingFeatures & features) == features))
			return format;
	}

	throw std::runtime_error("failed to find supported format!");
}

uvec2 DeviceImpl::maxImage2DSize() const noexcept {
	const auto &max_image_dimensions = m_physical_device.implementation()
	                                       .vkPhysicalDeviceProperties()
	                                       .limits.maxImageDimension2D;

	return {max_image_dimensions, max_image_dimensions};
}
