// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/render/Surface.hpp>
#include <storm/engine/vulkan/SurfaceImpl.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
Surface::Surface(const Device &device, const window::Window &window)
	: m_impl{device, window} {
	
}

/////////////////////////////////////
/////////////////////////////////////
void Surface::presentFrame(const Framebuffer &framebuffer, const Semaphore &render_finished_semaphore, const Fence &signal_fence) {
	m_impl->presentFrame(framebuffer, render_finished_semaphore, signal_fence);
}

/////////////////////////////////////
/////////////////////////////////////
Surface::~Surface() = default;

/////////////////////////////////////
/////////////////////////////////////
Surface::Surface(Surface &&) = default;

/////////////////////////////////////
/////////////////////////////////////
Surface &Surface::operator=(Surface &&) = default;
