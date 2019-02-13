// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/vulkan/CommandBufferImpl.hpp>
#include <storm/engine/vulkan/DeviceImpl.hpp>
#include <storm/engine/vulkan/FenceImpl.hpp>
#include <storm/engine/vulkan/FramebufferImpl.hpp>
#include <storm/engine/vulkan/SemaphoreImpl.hpp>
#include <storm/engine/vulkan/SurfaceImpl.hpp>
#include <storm/engine/vulkan/SwapChain.hpp>
#include <storm/log/LogOutput.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
SwapChain::SwapChain(const Device &device, const SurfaceImpl &surface)
    : m_device {device}, m_surface {surface} {
	createSwapChain();
	acquireSwapchainImages();
	createImageViews();
	createSemaphoresAndFences();
	createPresentCommandBuffers();
}

/////////////////////////////////////
/////////////////////////////////////
SwapChain::~SwapChain() = default;

/////////////////////////////////////
/////////////////////////////////////
SwapChain::SwapChain(SwapChain &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void SwapChain::presentFrame(const Framebuffer &framebuffer,
    const Semaphore &render_finished_semaphore, const Fence &signal_fence) {
	const auto &device_impl = m_device.implementation();
	const auto &vk_device   = device_impl.vkDevice();

	m_current_present_index
	    = (m_current_present_index + 1) % MAX_IMAGE_IN_FLIGHT;

	m_current_image_index
	    = vk_device
	          .acquireNextImageKHR(*m_swapchain,
	              std::numeric_limits<std::uint64_t>::max(),
	              m_image_available_semaphores[m_current_present_index]
	                  .implementation()
	                  .vkSemaphore(),
	              nullptr)
	          .value;

	{
		auto &present_command_buffer
		    = m_present_command_buffers[m_current_present_index];
		present_command_buffer.reset();
		present_command_buffer.begin();

		const auto sub_ressource
		    = vk::ImageSubresourceLayers {}
		          .setAspectMask(vk::ImageAspectFlagBits::eColor)
		          .setBaseArrayLayer(0)
		          .setMipLevel(0)
		          .setLayerCount(1);

		const auto &framebuffer_image
		    = framebuffer.implementation().backedVkImages()[0];

		// copy image to present queue image
		present_command_buffer.implementation().transitionImageLayout(
		    framebuffer_image.image.get(),
		    vk::ImageLayout::eColorAttachmentOptimal,
		    vk::ImageLayout::eTransferSrcOptimal,
		    vk::PipelineStageFlagBits::eColorAttachmentOutput,
		    vk::PipelineStageFlagBits::eTransfer);

		present_command_buffer.implementation().transitionImageLayout(
		    m_swapchain_images[m_current_image_index],
		    vk::ImageLayout::ePresentSrcKHR,
		    vk::ImageLayout::eTransferDstOptimal,
		    vk::PipelineStageFlagBits::eTransfer,
		    vk::PipelineStageFlagBits::eTransfer);

		present_command_buffer.implementation().blitImage(
		    framebuffer_image.image.get(),
		    m_swapchain_images[m_current_image_index], framebuffer_image.extent,
		    {m_extent.width, m_extent.height}, vk::Filter::eNearest);

		present_command_buffer.implementation().transitionImageLayout(
		    framebuffer_image.image.get(), vk::ImageLayout::eTransferSrcOptimal,
		    vk::ImageLayout::eColorAttachmentOptimal,
		    vk::PipelineStageFlagBits::eTransfer,
		    vk::PipelineStageFlagBits::eColorAttachmentOutput);

		present_command_buffer.implementation().transitionImageLayout(
		    m_swapchain_images[m_current_image_index],
		    vk::ImageLayout::eTransferDstOptimal,
		    vk::ImageLayout::ePresentSrcKHR,
		    vk::PipelineStageFlagBits::eTransfer,
		    vk::PipelineStageFlagBits::eTransfer);

		present_command_buffer.end();
		present_command_buffer.submit(
		    {&render_finished_semaphore,
		        &m_image_available_semaphores[m_current_present_index]},
		    {&m_render_finished_semaphores[m_current_present_index]},
		    {PipelineStage::COLOR_ATTACHMENT_OUTPUT,
		        PipelineStage::COLOR_ATTACHMENT_OUTPUT},
		    &signal_fence);
	}

	{
		const auto semaphores = std::array<vk::Semaphore, 1> {
		    m_render_finished_semaphores[m_current_present_index]
		        .implementation()
		        .vkSemaphore()};
		const auto wait_stages = std::array<vk::PipelineStageFlags, 1> {
		    vk::PipelineStageFlagBits::eColorAttachmentOutput};

		const auto present_info
		    = vk::PresentInfoKHR {}
		          .setWaitSemaphoreCount(std::size(semaphores))
		          .setPWaitSemaphores(std::data(semaphores))
		          .setSwapchainCount(1)
		          .setPSwapchains(&m_swapchain.get())
		          .setPImageIndices(&m_current_image_index);

		device_impl.vkGraphicsQueue().presentKHR(present_info);
	}
}

/////////////////////////////////////
/////////////////////////////////////
void SwapChain::createSwapChain() {
	const auto &vk_surface = m_surface.vkSurface();
	const auto &vk_device  = m_device.implementation().vkDevice();
	const auto  family_queue_indices
	    = m_device.implementation().familyQueuesIndices();

	// avoid layer error
	m_device.implementation()
	    .physicalDevice()
	    .implementation()
	    .vkPhysicalDevice()
	    .getSurfaceSupportKHR(
	        family_queue_indices.transfert_queue_index, vk_surface);

	selectFormat();
	selectExtent();
	selectPresentation();

	auto create_info
	    = vk::SwapchainCreateInfoKHR {}
	          .setSurface(vk_surface)
	          .setMinImageCount(m_image_count)
	          .setImageFormat(m_color_format)
	          .setImageColorSpace(m_color_space)
	          .setImageExtent(m_extent)
	          .setImageArrayLayers(1)
	          .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment
	                         | vk::ImageUsageFlagBits::eTransferDst)
	          .setImageSharingMode(vk::SharingMode::eExclusive)
	          .setPreTransform(m_pre_transform)
	          .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
	          .setClipped(true);

	if (family_queue_indices.graphics_queue_index
	    != family_queue_indices.transfert_queue_index) {
		const auto indices = std::array<std::uint32_t, 2> {
		    family_queue_indices.graphics_queue_index,
		    family_queue_indices.transfert_queue_index};

		create_info.setImageSharingMode(vk::SharingMode::eConcurrent)
		    .setQueueFamilyIndexCount(2)
		    .setPQueueFamilyIndices(std::data(indices));
	}

	m_swapchain = vk_device.createSwapchainKHRUnique(create_info);

	storm::DLOG("Renderer (vulkan)"_module, "Swapchain allocated at %{1}",
	    &m_swapchain.get());
}

/////////////////////////////////////
/////////////////////////////////////
void SwapChain::selectFormat() {
	const auto &surface_format = m_surface.vkSurfaceFormats();

	m_color_format = vk::Format {};
	if (std::size(surface_format) == 1
	    && surface_format[0].format == vk::Format::eUndefined)
		m_color_format = vk::Format::eB8G8R8Unorm;
	else
		m_color_format = surface_format[0].format;

	m_color_space = surface_format[0].colorSpace;
}

/////////////////////////////////////
/////////////////////////////////////
void SwapChain::selectExtent() {
	const auto &surface_capabilities = m_surface.vkSurfaceCapabilities();

	// TODO handle explicit double and triple buffering
	m_image_count = surface_capabilities.minImageCount + 1;
	if (surface_capabilities.maxImageCount > 0
	    && m_image_count > surface_capabilities.maxImageCount)
		m_image_count = surface_capabilities.maxImageCount;

	m_pre_transform = surface_capabilities.currentTransform;
	if (surface_capabilities.supportedTransforms
	    & vk::SurfaceTransformFlagBitsKHR::eIdentity)
		m_pre_transform = vk::SurfaceTransformFlagBitsKHR::eIdentity;

	if (surface_capabilities.currentExtent.width
	    == std::numeric_limits<std::uint32_t>::max()) {
		m_extent = m_surface.size();

		m_extent.width  = std::max(surface_capabilities.minImageExtent.width,
            std::min(
                surface_capabilities.maxImageExtent.width, m_extent.width));
		m_extent.height = std::max(surface_capabilities.minImageExtent.height,
		    std::min(
		        surface_capabilities.maxImageExtent.height, m_extent.height));
	} else
		m_extent = surface_capabilities.currentExtent;
}

/////////////////////////////////////
/////////////////////////////////////
void SwapChain::selectPresentation() {
	const auto &vk_present_mode = m_surface.vkSurfacePresentMode();

	auto best_mode = vk::PresentModeKHR::eFifo;

	for (const auto &present_mode : vk_present_mode) {
		if (present_mode == vk::PresentModeKHR::eMailbox) {
			m_present_mode = present_mode;
			return;
		} else if (present_mode == vk::PresentModeKHR::eImmediate)
			best_mode = present_mode;
	}

	m_present_mode = best_mode;
}

/////////////////////////////////////
/////////////////////////////////////
void SwapChain::acquireSwapchainImages() {
	const auto &vk_device = m_device.implementation().vkDevice();

	m_swapchain_images = vk_device.getSwapchainImagesKHR(*m_swapchain);

	auto cmd = m_device.createCommandBuffer();
	cmd.begin(true);
	for (const auto &image : m_swapchain_images) {
		cmd.implementation().transitionImageLayout(image,
		    vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR,
		    vk::PipelineStageFlagBits::eTopOfPipe,
		    vk::PipelineStageFlagBits::eTransfer);
	}
	cmd.end();

	cmd.submit({}, {});
	m_device.waitIdle();
}

/////////////////////////////////////
/////////////////////////////////////
void SwapChain::createImageViews() {
	const auto &vk_device = m_device.implementation().vkDevice();

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

	for (const auto &image : m_swapchain_images) {
		const auto create_info = vk::ImageViewCreateInfo {}
		                             .setImage(image)
		                             .setViewType(vk::ImageViewType::e2D)
		                             .setFormat(m_color_format)
		                             .setComponents(components)
		                             .setSubresourceRange(subressource_range);

		auto &image_view = m_swapchain_image_views.emplace_back(
		    vk_device.createImageViewUnique(create_info));
		storm::DLOG("Renderer (vulkan)"_module, "ImageView allocated at %{1}",
		    &image_view.get());
	}
}

/////////////////////////////////////
/////////////////////////////////////
void SwapChain::createSemaphoresAndFences() {
	const auto &vk_device = m_device.implementation().vkDevice();

	m_image_available_semaphores.reserve(MAX_IMAGE_IN_FLIGHT);
	m_render_finished_semaphores.reserve(MAX_IMAGE_IN_FLIGHT);
	m_in_flight_fences.reserve(MAX_IMAGE_IN_FLIGHT);
	for (auto i = 0u; i < MAX_IMAGE_IN_FLIGHT; ++i) {
		m_image_available_semaphores.emplace_back(m_device.createSemaphore());
		m_render_finished_semaphores.emplace_back(m_device.createSemaphore());
		m_in_flight_fences.emplace_back(m_device.createFence());
	}
}

/////////////////////////////////////
/////////////////////////////////////
void SwapChain::createPresentCommandBuffers() {
	m_present_command_buffers.reserve(MAX_IMAGE_IN_FLIGHT);

	for (auto i = 0u; i < MAX_IMAGE_IN_FLIGHT; ++i)
		m_present_command_buffers.emplace_back(m_device.createCommandBuffer());
}
