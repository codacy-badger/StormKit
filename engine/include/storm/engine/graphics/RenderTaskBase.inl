// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "RenderTaskBase.hpp"

namespace storm::engine {
	inline void RenderTaskBase::setCullImune(bool imune) {
		m_cull_imune = imune;
	}

	inline bool RenderTaskBase::cullImune() const noexcept {
		return m_cull_imune;
	}

	inline std::uint32_t RenderTaskBase::id() const noexcept { return m_id; }

	inline void RenderTaskBase::setName(std::string name) {
		m_name = std::move(name);
	}

	inline const std::string &RenderTaskBase::name() const noexcept {
		return m_name;
	}
}
