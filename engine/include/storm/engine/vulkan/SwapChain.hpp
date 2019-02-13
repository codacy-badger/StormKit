// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/Memory.hpp>
#include <storm/core/NonCopyable.hpp>
#include <storm/engine/vulkan/UniqueHandle.hpp>

namespace storm::engine {
	class Device;
	class SurfaceImpl;
	class CommandBuffer;
	class Semaphore;
	class Fence;
	class Framebuffer;
	class SwapChain : public core::NonCopyable {
	public:
		Unique_Object(SwapChain)

		    static constexpr const auto MAX_IMAGE_IN_FLIGHT = 3;

		explicit SwapChain(const Device &device, const SurfaceImpl &surface);
		~SwapChain();

		SwapChain(SwapChain &&);

		void presentFrame(const Framebuffer &framebuffer,
		    const Semaphore &                render_finished_semaphore,
		    const Fence &                    signal_fence);

	private:
		void createSwapChain();

		void selectFormat();
		void selectExtent();
		void selectPresentation();

		void acquireSwapchainImages();
		void createImageViews();

		void createSemaphoresAndFences();
		void createPresentCommandBuffers();

		UniqueHandle<vk::SwapchainKHR> m_swapchain;

		vk::Format                      m_color_format;
		vk::ColorSpaceKHR               m_color_space;
		vk::Extent2D                    m_extent;
		std::size_t                     m_image_count;
		vk::SurfaceTransformFlagBitsKHR m_pre_transform;
		vk::PresentModeKHR              m_present_mode;

		std::vector<vk::Image>                   m_swapchain_images;
		std::vector<UniqueHandle<vk::ImageView>> m_swapchain_image_views;

		std::uint32_t m_current_present_index;
		std::uint32_t m_current_image_index;

		std::vector<Semaphore>     m_image_available_semaphores;
		std::vector<Semaphore>     m_render_finished_semaphores;
		std::vector<Fence>         m_in_flight_fences;
		std::vector<CommandBuffer> m_present_command_buffers;

		const Device &     m_device;
		const SurfaceImpl &m_surface;
	};
}
