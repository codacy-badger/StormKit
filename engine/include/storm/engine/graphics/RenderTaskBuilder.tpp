// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "RenderGraph.hpp"
#include "RenderTaskBuilder.hpp"

namespace storm::engine {
    template <typename T>
	ResourceBase::ID RenderTaskBuilder::create(
      std::string name,
      typename T::ResourcePtr &&ptr
    ) {
		auto &create_resources = m_task.m_create_resources;

        auto id = m_pool.addTransientResource<T>(
            std::move(name),
            std::move(ptr), m_task);

		create_resources.emplace(id);

		return id;
	}

	template <typename Resource>
	ResourceBase::ID RenderTaskBuilder::write(ResourceBase::ID resourceID) {
		auto &write_resources = m_task.m_write_resources;

		auto &resource = m_pool.acquireResource(resourceID);
		resource.m_writers.emplace_back(m_task.id());

		write_resources.emplace(resourceID);

		return resource.id();
	}

	template <typename Resource>
	ResourceBase::ID RenderTaskBuilder::read(ResourceBase::ID resourceID) {
		auto &read_resources = m_task.m_read_resources;

		auto &resource = m_pool.acquireResource(resourceID);
		resource.m_readers.emplace_back(m_task.id());

		read_resources.emplace(resourceID);

		return resource.id();
	}
}
