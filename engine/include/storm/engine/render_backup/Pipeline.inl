#pragma once

#include "Pipeline.hpp"

namespace storm::engine {
	inline const PipelineInfo &Pipeline::infos() const noexcept {
		return m_infos;
	}
}
