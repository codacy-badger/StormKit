// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "SurfaceImpl.hpp"

namespace storm::engine {
    /////////////////////////////////////
    /////////////////////////////////////
    inline vk::Extent2D SurfaceImpl::size() const noexcept { return m_size; }

    /////////////////////////////////////
    /////////////////////////////////////
    inline const vk::SurfaceKHR &SurfaceImpl::vkSurface() const noexcept { return m_surface; }

    /////////////////////////////////////
    /////////////////////////////////////
    inline const vk::SurfaceCapabilitiesKHR &SurfaceImpl::vkSurfaceCapabilities() const noexcept {
        return m_surface_capabilities;
    }

    /////////////////////////////////////
    /////////////////////////////////////
    inline const std::vector<vk::PresentModeKHR> &SurfaceImpl::vkSurfacePresentMode() const
      noexcept {
        return m_present_modes;
    }

    /////////////////////////////////////
    /////////////////////////////////////
    inline const std::vector<vk::SurfaceFormatKHR> &SurfaceImpl::vkSurfaceFormats() const noexcept {
        return m_surface_formats;
    }
} // namespace storm::engine
