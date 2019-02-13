// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/graphics/ResourcePool.hpp>

namespace storm::engine {
	template <typename ResourceType, typename ResourceDescription>
	ResourceBase::ID ResourcePool::addRetainedResource(std::string name,
	    ResourceDescription &&description, ResourceType &resource) {
		using RawResourceDescription = std::decay_t<ResourceDescription>;
		using RawResourceType        = std::decay_t<ResourceType>;

		m_resources.emplace_back(
		    std::make_unique<Resource<ResourceDescription, ResourceType>>(
		        std::move(name), std::forward<ResourceDescription>(description),
		        resource));

		auto &resource_ = *m_resources.back();
		resource_.m_id  = m_next_id++;

		return resource_.id();
	}

	template <typename Resource, typename ResourceDescription>
	ResourceBase::ID ResourcePool::addTransientResource(const Device &device,
	    std::string name, ResourceDescription &&description,
	    std::optional<std::reference_wrapper<RenderTaskBase>> task) {
		m_resources.emplace_back(std::make_unique<Resource>(device,
		    std::move(name), std::forward<ResourceDescription>(description),
		    std::move(task)));

		auto &resource_ = *m_resources.back();
		resource_.m_id  = m_next_id++;

		return resource_.id();
	}

	template <typename T>
	const T &ResourcePool::acquireResourceAs(ResourceBase::ID resource_id) const
	    noexcept {
		return static_cast<const T &>(acquireResource(resource_id));
	}

	template <typename T>
	T &ResourcePool::acquireResourceAs(ResourceBase::ID resource_id) noexcept {
		return static_cast<T &>(acquireResource(resource_id));
	}
	
	template <typename T>
	const T *ResourcePool::acquireResourcePtrAs(ResourceBase::ID resource_id) const
	    noexcept {
		return dynamic_cast<const T *>(&acquireResource(resource_id));
	}

	template <typename T>
	T *ResourcePool::acquireResourcePtrAs(ResourceBase::ID resource_id) noexcept {
		return dynamic_cast<T *>(&acquireResource(resource_id));
	}
}
