// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "RenderTask.hpp"

namespace storm::engine {
	template <typename RenderTaskData>
	RenderTask<RenderTaskData>::RenderTask(std::string name,
										   SetupFunction setup_func,
										   ExecuteFunction execute_func)
		: RenderTaskBase{name},
		  m_setup_func{std::move(setup_func)},
		  m_execute_func{std::move(execute_func)} {

	}

	template <typename RenderTaskData>
	RenderTask<RenderTaskData>::~RenderTask() = default;

	template <typename RenderTaskData>
	RenderTask<RenderTaskData>::RenderTask(RenderTask &&) = default;

	template <typename RenderTaskData>
	RenderTask<RenderTaskData> &RenderTask<RenderTaskData>::operator=(RenderTask &&) = default;

	template <typename RenderTaskData>
	inline const RenderTaskData &RenderTask<RenderTaskData>::data() const noexcept {
		return m_data;
	}

	template <typename RenderTaskData>
	void RenderTask<RenderTaskData>::setup(RenderTaskBuilder &builder) {
		m_setup_func(m_data, builder);
	}

	template <typename RenderTaskData>
	void RenderTask<RenderTaskData>::execute(ResourcePool &pool) const {
		m_execute_func(m_data, pool);
	}
}
