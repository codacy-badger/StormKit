// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/NonCopyable.hpp>
#include <storm/engine/render/Context.hpp>
#include <storm/engine/vulkan/ContextImpl.hpp>
#include <storm/engine/vulkan/UniqueHandle.hpp>
#include <storm/sharedobject/SharedLibrary.hpp>

#if defined(VK_USE_PLATFORM_XCB_KHR)
static constexpr const auto SURFACE_EXTENSION_NAME = VK_KHR_XCB_SURFACE_EXTENSION_NAME;
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
static constexpr const auto SURFACE_EXTENSION_NAME = VK_MVK_MACOS_SURFACE_EXTENSION_NAME;
#endif

namespace storm::engine {
    class ContextImpl : public core::NonCopyable {
    public:
        static constexpr inline const auto DEVICE_EXTENSIONS = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        static constexpr inline const auto VALIDATION_LAYERS = {
          "VK_LAYER_LUNARG_standard_validation"};
        static constexpr inline const auto INSTANCE_EXTENSIONS = {VK_KHR_SURFACE_EXTENSION_NAME,
                                                                  SURFACE_EXTENSION_NAME};

        explicit ContextImpl(ContextSettings settings);
        ~ContextImpl();

        ContextImpl(ContextImpl &&);
        ContextImpl &operator=(ContextImpl &&);

        PhysicalDevice bestPhysicalDevice();

        inline const ContextSettings &settings() const noexcept;
        inline const vk::DispatchLoaderDynamic &dispatcher() const noexcept;
        inline const vk::Instance &instance() const noexcept;

    private:
        void createInstance();
        bool checkValidationLayerSupport(bool enable_validation);

        std::uint32_t scorePhysicalDevice(const vk::PhysicalDevice &device) const;
        vk::PhysicalDevice pickBestPhysicalDevice();

        ContextSettings m_context_settings;

        UniqueHandle<vk::Instance> m_instance;
        vk::DebugReportCallbackEXT m_callback;
        vk::DispatchLoaderDynamic m_dispatcher;

#ifndef STORM_OS_MACOS
        storm::sharedlibrary::SharedLibrary m_vulkan_shared_library;
#endif
    };
} // namespace storm::engine

#include "ContextImpl.inl"
