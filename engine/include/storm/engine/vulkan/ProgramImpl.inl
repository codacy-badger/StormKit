// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/vulkan/ProgramImpl.hpp>

namespace storm::engine {
	inline const vk::PipelineLayout &ProgramImpl::vkPipelineLayout() const noexcept {
		return m_pipeline_layout.get();
	}

	inline const std::vector<vk::DescriptorSetLayout> &ProgramImpl::vkDescriptorSetLayouts() const noexcept {
		return m_flat_descriptor_set_layouts;
	}
}
