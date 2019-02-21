// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/NonCopyable.hpp>
#include <storm/engine/render/Program.hpp>
#include <storm/engine/vulkan/UniqueHandle.hpp>

namespace storm::engine {
    class ProgramImpl : public core::NonCopyable {
    public:
        ProgramImpl(const Device &device);

        ProgramImpl(ProgramImpl &&);

        void addShaderModule(const Shader &module);
        const std::vector<const Shader *> &shaderModules() const noexcept;

        void link();

        inline const vk::PipelineLayout &vkPipelineLayout() const noexcept;
        inline const std::vector<vk::DescriptorSetLayout> &vkDescriptorSetLayouts() const noexcept;

    private:
        std::vector<const Shader *> m_shaders;

        UniqueHandle<vk::PipelineLayout> m_pipeline_layout;
        std::vector<UniqueHandle<vk::DescriptorSetLayout>> m_descriptor_set_layouts;
        std::vector<vk::DescriptorSetLayout> m_flat_descriptor_set_layouts;

        const Device &m_device;
    };
} // namespace storm::engine

#include "ProgramImpl.inl"
