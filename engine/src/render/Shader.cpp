// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/render/Shader.hpp>
#include <storm/engine/vulkan/ShaderImpl.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
Shader::Shader(const Device &device, DefaultShader shader)
    : m_impl {device, shader} {}

/////////////////////////////////////
/////////////////////////////////////
Shader::Shader(
    const Device &device, Stage stage, const _std::filesystem::path &path)
    : m_impl {device, stage, path} {}

/////////////////////////////////////
/////////////////////////////////////
Shader::Shader(
    const Device &device, Stage stage, const std::byte *data, std::size_t size)
    : m_impl {device, stage, data, size} {}

/////////////////////////////////////
/////////////////////////////////////
Shader::~Shader() = default;

/////////////////////////////////////
/////////////////////////////////////
Shader::Shader(Shader &&) = default;

/////////////////////////////////////
/////////////////////////////////////
Shader::Stage Shader::stage() const noexcept { return m_impl->stage(); }

/////////////////////////////////////
/////////////////////////////////////
const std::vector<std::byte> &Shader::source() const noexcept {
	return m_impl->source();
}
