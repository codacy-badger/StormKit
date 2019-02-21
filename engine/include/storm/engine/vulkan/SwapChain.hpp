// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <deque>
#include <storm/core/Memory.hpp>
#include <storm/core/NonCopyable.hpp>
#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/FrameToken.hpp>
#include <storm/engine/render/Semaphore.hpp>
#include <storm/engine/vulkan/DeviceImpl.hpp>
#include <storm/engine/vulkan/UniqueHandle.hpp>

namespace storm::engine {
    class SurfaceImpl;
    class SwapChain : public core::NonCopyable {
    public:
        Unique_Object(SwapChain)

          static constexpr const auto MAX_IMAGE_IN_FLIGHT = 3;

        explicit SwapChain(const Device &device, const SurfaceImpl &surface);
        ~SwapChain();

        SwapChain(SwapChain &&);

        FrameToken nextFrame();
        void present(Framebuffer &framebuffer, const FrameToken &token);

    private:
        void createSwapChain();

        void selectFormat();
        void selectExtent();
        void selectPresentation();

        void acquireSwapchainImages();
        void createSemaphoresAndFences();

        UniqueHandle<vk::SwapchainKHR> m_swapchain;

        vk::Format m_color_format;
        vk::ColorSpaceKHR m_color_space;
        vk::Extent2D m_extent;
        std::size_t m_image_count;
        vk::SurfaceTransformFlagBitsKHR m_pre_transform;
        vk::PresentModeKHR m_present_mode;

        std::vector<BackedSwapchainImage> m_images;
        std::vector<std::pair<Semaphore, Semaphore>> m_semaphores;
        std::vector<Fence> m_in_flight_fences;

        std::vector<Semaphore> m_present_semaphores;
        std::vector<CommandBuffer> m_present_cmd_buffers;

        std::uint32_t m_current_in_flight;
        std::uint64_t m_current_frame;

        const Device &m_device;
        const SurfaceImpl &m_surface;
    };
} // namespace storm::engine
