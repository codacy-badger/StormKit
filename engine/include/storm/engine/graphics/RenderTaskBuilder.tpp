// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "RenderTaskBuilder.hpp"
#include "RenderGraph.hpp"

namespace storm::engine {
	template <typename Resource, typename ResourceDescription>
	ResourceBase::ID RenderTaskBuilder::create(std::string name, ResourceDescription &&description) {
		static_assert(std::is_same_v<typename Resource::ResourceDescription, 
						std::remove_reference_t<ResourceDescription>>,
					  "Description does not match the resource.");;
		auto &create_resources = m_task.m_create_resources;

		auto id = m_pool.addTransientResource<Resource, ResourceDescription>(m_device,
											  std::move(name),
											  std::forward<ResourceDescription>(description),
											  m_task);

		create_resources.emplace(id);

		return id;
	}

	template <typename Resource>
	ResourceBase::ID RenderTaskBuilder::write(ResourceBase::ID resourceID) {
		auto &write_resources = m_task.m_write_resources;

		auto &resource = m_pool.acquireResource(resourceID);
		resource.m_writers.emplace(m_task.id());
		
		write_resources.emplace(resourceID);

		return resource.id();
	}

	template <typename Resource>
	ResourceBase::ID RenderTaskBuilder::read(ResourceBase::ID resourceID) {
		auto &read_resources = m_task.m_read_resources;

		auto &resource = m_pool.acquireResource(resourceID);
		resource.m_readers.emplace(m_task.id());
		
		read_resources.emplace(resourceID);

		return resource.id();
	}
}
