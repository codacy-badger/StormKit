// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/vulkan/ContextImpl.hpp>
#include <storm/engine/vulkan/DeviceImpl.hpp>
#include <storm/engine/vulkan/SurfaceImpl.hpp>
#include <storm/log/LogOutput.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
SurfaceImpl::SurfaceImpl(const Device &device, const window::Window &window)
    : m_device {device}, m_window {window} {
	createSurface();

	m_size = vk::Extent2D {m_window.size().w, m_window.size().h};

	m_swapchain = SwapChain::makeUnique(device, *this);
}

/////////////////////////////////////
/////////////////////////////////////
SurfaceImpl::~SurfaceImpl() {
	m_swapchain.reset();

	m_device.waitIdle();

	const auto &device      = m_device.implementation();
	const auto &context     = device.context().implementation();
	const auto &vk_instance = context.instance();

	vk_instance.destroySurfaceKHR(m_surface);
};

/////////////////////////////////////
/////////////////////////////////////
SurfaceImpl::SurfaceImpl(SurfaceImpl &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void SurfaceImpl::presentFrame(const Framebuffer &framebuffer,
    const Semaphore &render_finished_semaphore, const Fence &signal_fence) {
	m_swapchain->presentFrame(
	    framebuffer, render_finished_semaphore, signal_fence);
}

/////////////////////////////////////
/////////////////////////////////////
void SurfaceImpl::createSurface() {
	const auto &device      = m_device.implementation();
	const auto &context     = device.context().implementation();
	const auto &vk_instance = context.instance();
	const auto &vk_physical_device
	    = device.physicalDevice().implementation().vkPhysicalDevice();

#if defined(VK_USE_PLATFORM_XCB_KHR)
	struct Handles {
		xcb_connection_t *connection;
		xcb_window_t      window;
	} *handles = reinterpret_cast<Handles *>(m_window.nativeHandle());

	auto create_info = vk::XcbSurfaceCreateInfoKHR {}
	                       .setConnection(handles->connection)
	                       .setWindow(handles->window);

	m_surface = vk_instance.createXcbSurfaceKHR(
	    create_info, nullptr, context.dispatcher());
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
	auto create_info
	    = vk::MacOSSurfaceCreateInfoMVK {}.setPView(m_window.nativeHandle());

	m_surface = vk_instance.createMacOSSurfaceMVK(
	    create_info, nullptr, context.dispatcher());
#endif

	if (!m_surface)
		throw std::runtime_error("Failed to create Vulkan surface");

	storm::DLOG("Renderer (vulkan)"_module, "Render surface allocated at %{1}",
	    &m_surface);

	m_surface_capabilities
	    = vk_physical_device.getSurfaceCapabilitiesKHR(m_surface);
	m_surface_formats = vk_physical_device.getSurfaceFormatsKHR(m_surface);
	m_present_modes   = vk_physical_device.getSurfacePresentModesKHR(m_surface);
}
