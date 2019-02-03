// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "RenderTaskBuilder.hpp"
#include "RenderGraph.hpp"

namespace storm::engine {
	template <typename Resource, typename ResourceDescription>
	Resource *RenderTaskBuilder::create(std::string name, ResourceDescription &&description) {
		static_assert(std::is_same_v<typename Resource::ResourceDescription, std::remove_reference_t<ResourceDescription>>,
					  "Description does not match the resource.");
		auto &resources = m_graph.m_resources;
		auto &create_resources = m_task.m_create_resources;

		auto resource_ptr = std::make_unique<Resource>(
					m_device,
					std::move(name),
					std::forward<typename Resource::ResourceDescription>(description),
					&(m_task));

		resources.emplace_back(std::move(resource_ptr));
		auto *resource = resources.back().get();

		create_resources.emplace_back(resource);

		return static_cast<Resource*>(resource);
	}

	template <typename Resource>
	Resource *RenderTaskBuilder::write(Resource &resource) {
		auto &write_resources = m_task.m_write_resources;

		resource.m_writers.emplace_back(&(m_task));
		write_resources.emplace_back(&resource);

		return &resource;
	}

	template <typename Resource>
	Resource *RenderTaskBuilder::read(Resource &resource) {
		auto &read_resources = m_task.m_read_resources;

		resource.m_readers.emplace_back(&(m_task));
		read_resources.emplace_back(&resource);

		return &resource;
	}
}
