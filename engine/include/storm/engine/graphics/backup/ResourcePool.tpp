// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/graphics/ResourcePool.hpp>

namespace storm::engine {
	template <typename ResourceDescription, typename ResourceType>
	ResourceBase::ID ResourcePool::addRetainedResource(std::string name,
	    ResourceDescription &&description, ResourceType &resource) {
		m_resources.emplace_back(
		    Resource<ResourceDescription, ResourceType>::makeUnique(
		        std::move(name), std::forward<ResourceDescription>(description),
		        std::ref(resource)));

		auto &resource_ = *m_resources.back();
		resource_.m_id  = m_next_id++;

		return resource_.id();
	}

	template <typename Resource, typename Description>
	ResourceBase::ID ResourcePool::addTransientResource(const Device &device,
	    std::string name, Description &&description,
	    std::optional<std::reference_wrapper<RenderTaskBase>> task) {

		m_resources.emplace_back(Resource::makeUnique(device, std::move(name),
		    std::forward<Description>(description), std::move(task)));

		auto &resource_ = *m_resources.back();
		resource_.m_id  = m_next_id++;

		return resource_.id();
	}
}
