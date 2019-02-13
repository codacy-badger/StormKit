// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <spirv_cross.hpp>
#include <storm/engine/vulkan/DeviceImpl.hpp>
#include <storm/engine/vulkan/ProgramImpl.hpp>
#include <storm/log/LogOutput.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
ProgramImpl::ProgramImpl(const Device &device) : m_device {device} {}

/////////////////////////////////////
/////////////////////////////////////
ProgramImpl::ProgramImpl(ProgramImpl &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void ProgramImpl::addShaderModule(const Shader &module) {
	m_shaders.emplace_back(&module);
}

/////////////////////////////////////
/////////////////////////////////////
const std::vector<const Shader *> &ProgramImpl::shaderModules() const noexcept {
	return m_shaders;
}

/////////////////////////////////////
/////////////////////////////////////
void ProgramImpl::link() {
	const auto &vk_device     = m_device.implementation().vkDevice();
	auto        vertex_shader = std::find_if(
        std::begin(m_shaders), std::end(m_shaders), [](const auto &shader) {
            return shader->stage() == Shader::Stage::VERTEX;
        });

	auto descriptor_layout_bindings
	    = std::vector<vk::DescriptorSetLayoutBinding> {};
	if (vertex_shader != std::end(m_shaders)) {
		auto spirv_binary     = (*vertex_shader)->source();
		auto spirv_binary_int = std::vector<std::uint32_t> {};
		spirv_binary_int.resize(
		    std::size(spirv_binary) / sizeof(std::uint32_t), 0);

		std::memcpy(std::data(spirv_binary_int), std::data(spirv_binary),
		    std::size(spirv_binary));

		auto compiler = spirv_cross::Compiler {std::move(spirv_binary_int)};
		auto shader_resources = compiler.get_shader_resources();

		for (const auto &uniform : shader_resources.uniform_buffers) {
			const auto set = compiler.get_decoration(
			    uniform.id, spv::DecorationDescriptorSet);
			const auto binding
			    = compiler.get_decoration(uniform.id, spv::DecorationBinding);

			const auto descriptor_layout_binding
			    = vk::DescriptorSetLayoutBinding {}
			          .setBinding(binding)
			          .setStageFlags(vk::ShaderStageFlagBits::eVertex)
			          .setDescriptorType(vk::DescriptorType::eUniformBuffer)
			          .setDescriptorCount(1);

			descriptor_layout_bindings.emplace_back(
			    std::move(descriptor_layout_binding));
		}
	}

	auto fragment_shader = std::find_if(
	    std::begin(m_shaders), std::end(m_shaders), [](const auto &shader) {
		    return shader->stage() == Shader::Stage::FRAGMENT;
	    });
	if (fragment_shader != std::end(m_shaders)) {
		auto spirv_binary     = (*fragment_shader)->source();
		auto spirv_binary_int = std::vector<std::uint32_t> {};
		spirv_binary_int.resize(
		    std::size(spirv_binary) / sizeof(std::uint32_t), 0);

		std::memcpy(std::data(spirv_binary_int), std::data(spirv_binary),
		    std::size(spirv_binary));

		auto compiler = spirv_cross::Compiler {std::move(spirv_binary_int)};
		auto shader_resources = compiler.get_shader_resources();

		for (const auto &image : shader_resources.sampled_images) {
			const auto set = compiler.get_decoration(
			    image.id, spv::DecorationDescriptorSet);
			const auto binding
			    = compiler.get_decoration(image.id, spv::DecorationBinding);

			const auto descriptor_layout_binding
			    = vk::DescriptorSetLayoutBinding {}
			          .setBinding(binding)
			          .setStageFlags(vk::ShaderStageFlagBits::eFragment)
			          .setDescriptorType(
			              vk::DescriptorType::eCombinedImageSampler)
			          .setDescriptorCount(1);

			descriptor_layout_bindings.emplace_back(
			    std::move(descriptor_layout_binding));
		}
	}

	const auto descriptor_set_layout_create_info
	    = vk::DescriptorSetLayoutCreateInfo {}
	          .setBindingCount(std::size(descriptor_layout_bindings))
	          .setPBindings(std::data(descriptor_layout_bindings));

	m_descriptor_set_layouts.emplace_back(
	    vk_device.createDescriptorSetLayoutUnique(
	        descriptor_set_layout_create_info));

	m_flat_descriptor_set_layouts.reserve(std::size(m_descriptor_set_layouts));
	for (const auto &descriptor_set_layout : m_descriptor_set_layouts)
		m_flat_descriptor_set_layouts.emplace_back(descriptor_set_layout.get());

	const auto pipeline_layout_create_info
	    = vk::PipelineLayoutCreateInfo {}
	          .setSetLayoutCount(std::size(m_flat_descriptor_set_layouts))
	          .setPSetLayouts(std::data(m_flat_descriptor_set_layouts));

	m_pipeline_layout
	    = vk_device.createPipelineLayoutUnique(pipeline_layout_create_info);
}
