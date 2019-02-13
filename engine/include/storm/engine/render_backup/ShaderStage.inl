#pragma once

#include "ShaderStage.hpp"

namespace storm::engine {
	inline const _std::filesystem::path &ShaderStage::filepath() const
	    noexcept {
		return m_filepath;
	}

	inline const std::vector<std::byte> &ShaderStage::source() const noexcept {
		return m_source;
	}

	inline storm::engine::ShaderType ShaderStage::type() const noexcept {
		return m_type;
	}
}
