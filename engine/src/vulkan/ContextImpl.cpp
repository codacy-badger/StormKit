// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/vulkan/ContextImpl.hpp>
#include <storm/engine/vulkan/PhysicalDeviceImpl.hpp>

#include <storm/log/LogOutput.hpp>

#include <storm/core/Configure.hpp>

#include <map>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
VKAPI_ATTR VkBool32 debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char *layerPrefix, const char *msg, [[maybe_unused]] void *userData) {
	static const constexpr auto MESSAGE = "Layer: %{1} | Code: %{2}\n Message: %{3}";
	switch(flags) {
		case VK_DEBUG_REPORT_DEBUG_BIT_EXT:
			storm::DLOG("Renderer (vulkan-layer)"_module, MESSAGE, layerPrefix, code, msg);
			break;
		case VK_DEBUG_REPORT_ERROR_BIT_EXT:
			storm::ELOG("Renderer (vulkan-layer)"_module, MESSAGE, layerPrefix, code, msg);
			break;
		case VK_DEBUG_REPORT_WARNING_BIT_EXT:
			storm::WLOG("Renderer (vulkan-layer)"_module, MESSAGE, layerPrefix, code, msg);
			break;
		/*case VK_DEBUG_REPORT_INFORMATION_BIT_EXT:
			storm::ILOG("Vulkan"_module, MESSAGE, layerPrefix, code, msg);
			break;*/
	}

	return VK_FALSE;
}

/////////////////////////////////////
/////////////////////////////////////
ContextImpl::ContextImpl(ContextSettings settings)
	: m_context_settings{std::move(settings)} {
	createInstance();
}

/////////////////////////////////////
/////////////////////////////////////
ContextImpl::~ContextImpl() = default;

/////////////////////////////////////
/////////////////////////////////////
ContextImpl::ContextImpl(ContextImpl &&) = default;

/////////////////////////////////////
/////////////////////////////////////
ContextImpl &ContextImpl::operator=(ContextImpl &&) = default;

/////////////////////////////////////
/////////////////////////////////////
PhysicalDevice ContextImpl::bestPhysicalDevice() {
	auto vk_physical_device = pickBestPhysicalDevice();
	
	auto physical_device = PhysicalDevice{};
	physical_device.m_impl.init(std::move(vk_physical_device));
	
	return physical_device;
}

/////////////////////////////////////
/////////////////////////////////////
void ContextImpl::createInstance() {
	const auto vk_app_version   = vkMakeVersion(m_context_settings.app_version.major,
												m_context_settings.app_version.minor,
												m_context_settings.app_version.patch);
	const auto vk_storm_version = vkMakeVersion(STORM_MAJOR_VERSION, 
												STORM_MINOR_VERSION, 
												STORM_PATCH_VERSION);
	
	auto app_info = vk::ApplicationInfo{}
		.setApiVersion(VK_API_VERSION_1_0)
		.setPApplicationName(m_context_settings.app_name.data())
		.setApplicationVersion(vk_app_version)
		.setPEngineName("StormKit C++")
		.setEngineVersion(vk_storm_version);

	auto create_info = vk::InstanceCreateInfo{}
				.setPApplicationInfo(&app_info)
				.setEnabledLayerCount(0);

	auto instance_extensions = std::vector<const char*>{ INSTANCE_EXTENSIONS };
	const auto enable_validation = checkValidationLayerSupport(m_context_settings.enable_validation);
	if(enable_validation) {
		create_info.setEnabledLayerCount(static_cast<std::uint32_t>(std::size(VALIDATION_LAYERS)))
				   .setPpEnabledLayerNames(std::data(VALIDATION_LAYERS));

		instance_extensions.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	create_info.setEnabledExtensionCount(static_cast<std::uint32_t>(std::size(instance_extensions)))
			   .setPpEnabledExtensionNames(std::data(instance_extensions));

	m_instance = vk::createInstanceUnique(create_info);
	if(!m_instance)
		throw std::runtime_error("Failed to create Vulkan instance!");

	m_dispatcher = vk::DispatchLoaderDynamic{*m_instance};

	if(enable_validation) {
		auto debug_create_info = vk::DebugReportCallbackCreateInfoEXT{}
				.setFlags(vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning | vk::DebugReportFlagBitsEXT::eDebug | vk::DebugReportFlagBitsEXT::eInformation)
				.setPfnCallback(debugCallback);

		m_dispatcher.vkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(m_instance->getProcAddr("vkCreateDebugReportCallbackEXT"));
		m_dispatcher.vkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(m_instance->getProcAddr("vkDestroyDebugReportCallbackEXT"));

		m_callback = m_instance->createDebugReportCallbackEXT(debug_create_info, nullptr, m_dispatcher);
		if(!m_callback)
			throw std::runtime_error("Failed to create Vulkan debug report output");
	}

	storm::DLOG("Renderer (vulkan)"_module, "Vulkan Instance allocated at %{1}", &m_instance.get());
}

/////////////////////////////////////
/////////////////////////////////////
bool ContextImpl::checkValidationLayerSupport(bool enable_validation) {
	const auto available_layers = vk::enumerateInstanceLayerProperties();

	for (auto layer_name : std::as_const(VALIDATION_LAYERS)) {
		auto layer_found = false;

		for (const auto& layer_properties : available_layers) {
			if (std::strcmp(layer_name, layer_properties.layerName) == 0) {
				layer_found = true;
				break;
			}
		}

		if (!layer_found)
			return false;
	}

	return enable_validation;
}

/////////////////////////////////////
/////////////////////////////////////
std::uint32_t ContextImpl::scorePhysicalDevice(const vk::PhysicalDevice &device) const {
	auto score = 0u;

	const auto device_extensions = device.enumerateDeviceExtensionProperties();
	for(const auto &needed_extension : std::as_const(DEVICE_EXTENSIONS))  {
		auto has_extension = false;

		for(const auto &device_extension : std::as_const(device_extensions)) {
			has_extension = std::strcmp(device_extension.extensionName, needed_extension) == 0;

			if(has_extension) break;
		}

		if(!has_extension) return 0u;
	}

	const auto properties = device.getProperties();
	//auto features   = device.getFeatures();
	if(properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
		score += 1000u;
	
	score += properties.limits.maxImageDimension2D;
	

	return score;
}

/////////////////////////////////////
/////////////////////////////////////
vk::PhysicalDevice ContextImpl::pickBestPhysicalDevice() {
	auto ranked_devices = std::multimap<std::uint32_t, vk::PhysicalDevice>{};

	static const auto devices = m_instance->enumeratePhysicalDevices();

	for(const auto &device : std::as_const(devices)) {
		const auto score = scorePhysicalDevice(device);
		ranked_devices.emplace(score, device);
	}

	if(ranked_devices.rbegin()->first > 0u) {
		auto &physical_device = ranked_devices.rbegin()->second;

		return physical_device;	
	}
}
