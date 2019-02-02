// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/vulkan/PhysicalDeviceImpl.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
PhysicalDeviceImpl::PhysicalDeviceImpl(vk::PhysicalDevice device)
	: m_physical_device{std::move(device)} {
	m_physical_device_properties        = m_physical_device.getProperties();
	m_physical_device_features          = m_physical_device.getFeatures();
	m_physical_device_memory_properties = m_physical_device.getMemoryProperties();	
}

/////////////////////////////////////
/////////////////////////////////////
PhysicalDeviceImpl::~PhysicalDeviceImpl()  = default;

/////////////////////////////////////
/////////////////////////////////////
PhysicalDeviceImpl::PhysicalDeviceImpl(PhysicalDeviceImpl &&) = default;

/////////////////////////////////////
/////////////////////////////////////
PhysicalDeviceImpl &PhysicalDeviceImpl::operator=(PhysicalDeviceImpl &&)  = default;
