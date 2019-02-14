// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <fstream>
#include <storm/engine/vulkan/DeviceImpl.hpp>
#include <storm/engine/vulkan/ShaderImpl.hpp>
#include <storm/log/LogOutput.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
ShaderImpl::ShaderImpl(const Device &device, Shader::Stage stage,
    const _std::filesystem::path &path)
    : m_stage {stage}, m_device {device} {
	loadFromFile(path);
}

/////////////////////////////////////
/////////////////////////////////////
ShaderImpl::ShaderImpl(const Device &device, Shader::Stage stage,
    const std::byte *data, std::size_t size)
    : m_stage {stage}, m_device {device} {
	loadFromMemory(data, size);
}

/////////////////////////////////////
/////////////////////////////////////
ShaderImpl::~ShaderImpl() = default;

/////////////////////////////////////
/////////////////////////////////////
ShaderImpl::ShaderImpl(ShaderImpl &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void ShaderImpl::loadFromFile(const _std::filesystem::path &path) {
	auto data = std::vector<char> {};

	{
		auto file
		    = std::ifstream {path.string(), std::ios::ate | std::ios::binary};
		const auto file_size = file.tellg();

		file.seekg(0);

		data.resize(file_size);

		file.read(std::data(data), file_size);
	}

	loadFromMemory(
	    reinterpret_cast<std::byte *>(std::data(data)), std::size(data));
}

/////////////////////////////////////
/////////////////////////////////////
void ShaderImpl::loadFromMemory(const std::byte *data, std::size_t size) {
	const auto &vk_device = m_device.implementation().vkDevice();

	const auto create_info
	    = vk::ShaderModuleCreateInfo {}.setCodeSize(size).setPCode(
	        reinterpret_cast<const std::uint32_t *>(data));

	m_shader_module = vk_device.createShaderModuleUnique(create_info);

	storm::DLOG("Renderer (vulkan)"_module, "Shader module allocated at %{1}",
	    &m_shader_module.get());

	m_source.resize(size);
	std::copy(data, data + size, std::data(m_source));
}
