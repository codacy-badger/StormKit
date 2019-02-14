// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/render/Shader.hpp>
#include <storm/engine/vulkan/UniqueHandle.hpp>

namespace storm::engine {
	class ShaderImpl {
	public:
		explicit ShaderImpl(const Device &device, Shader::Stage stage,
		    const _std::filesystem::path &path);
		explicit ShaderImpl(const Device &device, Shader::Stage stage,
		    const std::byte *data, std::size_t size);
		~ShaderImpl();

		ShaderImpl(ShaderImpl &&);

		inline Shader::Stage                 stage() const noexcept;
		inline const std::vector<std::byte> &source() const noexcept;

		inline const vk::ShaderModule &vkShaderModule() const noexcept;

	private:
		void loadFromFile(const _std::filesystem::path &path);
		void loadFromMemory(const std::byte *data, std::size_t size);

		Shader::Stage          m_stage;
		std::vector<std::byte> m_source;

		UniqueHandle<vk::ShaderModule> m_shader_module;

		const Device &m_device;
	};
}

#include "ShaderImpl.inl"
