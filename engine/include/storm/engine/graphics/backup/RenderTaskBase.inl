// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "RenderTaskBase.hpp"

namespace storm::engine {
	inline std::uint32_t RenderTaskBase::id() const noexcept { return m_id; }

	inline void RenderTaskBase::setName(std::string name) {
		m_name = std::move(name);
	}

	inline const std::string &RenderTaskBase::name() const noexcept {
		return m_name;
	}

	inline const Framebuffer &RenderTaskBase::framebuffer() const noexcept {
		return *m_framebuffer;
	}

	inline PipelineState &RenderTaskBase::pipelineState() noexcept {
		return m_pipeline_state;
	}
}
