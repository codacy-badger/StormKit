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

		auto builder = RenderTaskBuilder{m_device, task_ref, *this};
		task_ref.setup(builder);

		return static_cast<RenderTask<RenderTaskData>&>(task_ref);
	}

	template <typename ResourceDescription, typename ResourceType>
	Resource<std::remove_reference_t<ResourceDescription>, std::remove_reference_t<ResourceType>> *
		RenderGraph::addRetainedResource(std::string name,
										ResourceDescription &&description,
										ResourceType *resource) {
		auto resource_ptr = std::make_unique<Resource<ResourceDescription, ResourceType>>(
					std::move(name), std::forward<ResourceDescription>(description), resource);

		m_resources.emplace_back(std::move(resource_ptr));

		return static_cast<Resource<std::remove_reference_t<ResourceDescription>, std::remove_reference_t<ResourceType>>*>(m_resources.back().get());
	}
}
