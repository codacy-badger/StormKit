// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/vulkan/CommandBufferImpl.hpp>

namespace storm::engine {
	inline PipelineState &CommandBufferImpl::pipelineState() noexcept {
		m_is_pipeline_state_dirty = true;
		return m_pipeline_state;
	}

	inline const PipelineState &CommandBufferImpl::pipelineState() const noexcept {
		return m_pipeline_state;
	}

	inline BindingState &CommandBufferImpl::bindingState() noexcept {
		m_is_binding_state_dirty = true;
		return m_binding_state;
	}

	inline const BindingState &CommandBufferImpl::bindingState() const noexcept {
		return m_binding_state;
	}

	inline const vk::CommandBuffer &CommandBufferImpl::vkCommandBuffer() const noexcept {
		return m_command_buffer.get();
	}
}
