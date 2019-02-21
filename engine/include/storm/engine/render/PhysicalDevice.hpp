// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/NonCopyable.hpp>
#include <storm/core/Pimpl.hpp>
#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/Utils.hpp>

namespace storm::engine {
    class PhysicalDeviceImpl;
    class PhysicalDevice : public core::NonCopyable {
    public:
        ~PhysicalDevice();

        PhysicalDevice(PhysicalDevice &&);
        PhysicalDevice &operator=(PhysicalDevice &&);

        IMPLEMENTATION(PhysicalDeviceImpl)
    private:
        explicit PhysicalDevice();

        core::Pimpl<PhysicalDeviceImpl, false> m_impl;

        friend class ContextImpl;
    };
} // namespace storm::engine
