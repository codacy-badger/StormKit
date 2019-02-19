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
	: m_device {device}, m_surface {surface}, m_current_in_flight{0u}, m_current_frame{0u} {
	createSwapChain();
	acquireSwapchainImages();
	createSemaphoresAndFences();
}

/////////////////////////////////////
/////////////////////////////////////
SwapChain::~SwapChain() = default;

/////////////////////////////////////
/////////////////////////////////////
SwapChain::SwapChain(SwapChain &&) = default;

/////////////////////////////////////
/////////////////////////////////////
FrameToken SwapChain::nextFrame() {
	auto &fence = m_in_flight_fences[m_current_in_flight];
	fence.wait();
	fence.reset();

	const auto &device_impl = m_device.implementation();
	const auto &vk_device   = device_impl.vkDevice();

	auto &[image_available, render_finished] = m_semaphores[m_current_in_flight];

	const auto image_index = vk_device.acquireNextImageKHR(
			*m_swapchain,
			std::numeric_limits<std::uint64_t>::max(),
			image_available.implementation().vkSemaphore(),
			nullptr
	).value;

	return FrameToken {
		m_current_frame,
		image_index,
		std::size(m_images),
		image_available,
		render_finished,
		fence
	};
}


/////////////////////////////////////
/////////////////////////////////////
void SwapChain::present(Framebuffer &framebuffer, const FrameToken &token) {
	const auto &device_impl = m_device.implementation();
	const auto &vk_device   = device_impl.vkDevice();

	auto &present_command_buffer = m_present_cmd_buffers[m_current_in_flight];
	present_command_buffer.reset();
	present_command_buffer.begin();

	const auto sub_ressource = vk::ImageSubresourceLayers{}
		.setAspectMask(vk::ImageAspectFlagBits::eColor)
		.setBaseArrayLayer(0)
		.setMipLevel(0)
		.setLayerCount(1);

    const auto &framebuffer_image =
      framebuffer.implementation().outputTextures().begin()->second.implementation().backedVkTexture().image;

	//copy image to present queue image
	present_command_buffer.implementation().transitionImageLayout(
		framebuffer_image.image.get(),
		vk::ImageLayout::eColorAttachmentOptimal,
		vk::ImageLayout::eTransferSrcOptimal,
		vk::PipelineStageFlagBits::eColorAttachmentOutput,
		vk::PipelineStageFlagBits::eTransfer
	);

	present_command_buffer.implementation().transitionImageLayout(
		m_images[token.image_index].image,
		vk::ImageLayout::ePresentSrcKHR,
		vk::ImageLayout::eTransferDstOptimal,
		vk::PipelineStageFlagBits::eTransfer,
		vk::PipelineStageFlagBits::eTransfer
	);

	present_command_buffer.implementation().blitImage(
		framebuffer_image.image.get(),
		m_images[token.image_index].image,
		framebuffer_image.extent,
		{m_extent.width, m_extent.height},
		vk::Filter::eNearest
	);

	present_command_buffer.implementation().transitionImageLayout(
		framebuffer_image.image.get(),
		vk::ImageLayout::eTransferSrcOptimal,
		vk::ImageLayout::eColorAttachmentOptimal,
		vk::PipelineStageFlagBits::eTransfer,
		vk::PipelineStageFlagBits::eColorAttachmentOutput
	);

	present_command_buffer.implementation().transitionImageLayout(
		m_images[token.image_index].image,
		vk::ImageLayout::eTransferDstOptimal,
		vk::ImageLayout::ePresentSrcKHR,
		vk::PipelineStageFlagBits::eTransfer,
		vk::PipelineStageFlagBits::eTransfer
	);

	present_command_buffer.end();

	present_command_buffer.submit(
		{
			&token.render_finished,
			&token.image_available
		},
		{&m_present_semaphores[m_current_in_flight]},
		QueueType::TRANSFERT,
		{PipelineStage::COLOR_ATTACHMENT_OUTPUT, PipelineStage::COLOR_ATTACHMENT_OUTPUT},
		&token.fence
	);

	device_impl.vkGraphicsQueue().presentKHR(
		vk::PresentInfoKHR{}
		.setSwapchainCount(1)
		.setPSwapchains(&m_swapchain.get())
		.setPImageIndices(&token.image_index)
		.setWaitSemaphoreCount(1)
		.setPWaitSemaphores(&m_present_semaphores[m_current_in_flight].implementation().vkSemaphore())
	);

	m_current_in_flight = (m_current_in_flight + 1) % std::size(m_images);
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

	auto cmd = m_device.createCommandBuffer();
	cmd.begin(true);
	for(auto image : vk_device.getSwapchainImagesKHR(*m_swapchain)) {
		m_images.emplace_back(m_device.implementation().createBackedSwapchainImage(m_color_format, image));

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
void SwapChain::createSemaphoresAndFences() {
	const auto &vk_device = m_device.implementation().vkDevice();

	const auto image_count = std::size(m_images);

	m_semaphores.reserve(image_count );
	m_in_flight_fences.reserve(image_count );
	for (auto i = 0u; i < image_count; ++i) {
		m_semaphores.emplace_back(m_device.createSemaphore(), m_device.createSemaphore());
		m_present_semaphores.emplace_back(m_device.createSemaphore());
		m_in_flight_fences.emplace_back(m_device.createFence());
		m_present_cmd_buffers.emplace_back(m_device.createCommandBuffer());
	}
}
