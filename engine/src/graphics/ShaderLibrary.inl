// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "ShaderLibrary.hpp"

namespace storm::engine {
	inline const std::vector<std::byte> &ShaderLibrary::getSource(
	    std::string_view name) {
		return m_sources[name];
	}
}
