// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "RenderGraph.hpp"

namespace storm::engine {
	template <typename RenderTaskData>
	RenderTask<RenderTaskData> &RenderGraph::addRenderPass(const std::string name,
														   typename RenderTask<RenderTaskData>::SetupFunction setup_func,
														   typename RenderTask<RenderTaskData>::ExecuteFunction execute_func) {
		auto task = std::make_unique<RenderTask<RenderTaskData>>(name, setup_func, execute_func);

		m_render_tasks.emplace_back(std::move(task));

		auto &task_ref = *m_render_tasks.back();

		auto builder = RenderTaskBuilder{m_device, task_ref, m_resources};
		task_ref.m_id = m_next_task_id++;
		task_ref.setup(builder);

		return static_cast<RenderTask<RenderTaskData>&>(task_ref);
	}

	template <typename ResourceType, typename ResourceDescription>
	std::uint32_t RenderGraph::addRetainedResource(std::string name,
										ResourceDescription &&description,
										ResourceType &resource) {
		return m_resources.addRetainedResource<ResourceType, ResourceDescription>(
					std::move(name), 
					std::forward<ResourceDescription>(description), 
					resource
				);
	}
	
	template <typename T>
	const T &RenderGraph::getRenderTaskAs(RenderTaskBase::ID render_task_id) const noexcept {
		//static_assert (std::is_, )
		
		return static_cast<T&>(getRenderTask(render_task_id));
	}
	
	template <typename T>
	T &RenderGraph::getRenderTaskAs(RenderTaskBase::ID render_task_id) noexcept {
		return static_cast<T&>(getRenderTask(render_task_id));
	}
}
