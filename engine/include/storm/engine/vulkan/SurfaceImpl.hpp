// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/NonCopyable.hpp>
#include <storm/engine/render/Surface.hpp>
#include <storm/engine/vulkan/SwapChain.hpp>
#include <storm/engine/vulkan/UniqueHandle.hpp>
#include <storm/window/Window.hpp>

namespace storm::engine {
	class SurfaceImpl : public core::NonCopyable {
	public:
		explicit SurfaceImpl(
		    const Device &device, const window::Window &window);
		~SurfaceImpl();

		SurfaceImpl(SurfaceImpl &&);

		void presentFrame(const Framebuffer &framebuffer,
		    const Semaphore &                render_finished_semaphore,
		    const Fence &                    signal_fence);

		inline vk::Extent2D size() const noexcept;

		inline const vk::SurfaceKHR &            vkSurface() const noexcept;
		inline const vk::SurfaceCapabilitiesKHR &vkSurfaceCapabilities() const
		    noexcept;
		inline const std::vector<vk::PresentModeKHR> &
		                                                vkSurfacePresentMode() const noexcept;
		inline const std::vector<vk::SurfaceFormatKHR> &vkSurfaceFormats() const
		    noexcept;

	private:
		void createSurface();

		vk::SurfaceKHR                    m_surface;
		vk::SurfaceCapabilitiesKHR        m_surface_capabilities;
		std::vector<vk::SurfaceFormatKHR> m_surface_formats;
		std::vector<vk::PresentModeKHR>   m_present_modes;

		vk::Extent2D m_size;

		SwapChain::Ptr m_swapchain;

		const Device &        m_device;
		const window::Window &m_window;
	};
}

#include "SurfaceImpl.inl"
