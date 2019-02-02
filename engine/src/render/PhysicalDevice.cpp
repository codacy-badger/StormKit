// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/render/PhysicalDevice.hpp>
#include <storm/engine/vulkan/PhysicalDeviceImpl.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
PhysicalDevice::PhysicalDevice()
	: m_impl{true} {
	
}

/////////////////////////////////////
/////////////////////////////////////
PhysicalDevice::~PhysicalDevice() = default;

/////////////////////////////////////
/////////////////////////////////////
PhysicalDevice::PhysicalDevice(PhysicalDevice &&) = default;

/////////////////////////////////////
/////////////////////////////////////
PhysicalDevice &PhysicalDevice::operator=(PhysicalDevice &&) = default;
