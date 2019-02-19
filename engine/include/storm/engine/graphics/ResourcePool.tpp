// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/graphics/ResourcePool.hpp>

namespace storm::engine {
    template <typename T>
    ResourceBase::ID ResourcePool::addRetainedResource(
      std::string name,
      T &resource
    ) {
        auto resource_ptr =
          std::make_unique<Resource<T>>(std::move(name), resource);

        resource_ptr->m_id = m_next_id++;
        const auto resource_id = resource_ptr->id();

        m_resources.emplace_back(std::move(resource_ptr));

        return resource_id;
    }

    template <typename T>
    ResourceBase::ID ResourcePool::addTransientResource(
        std::string name,
        typename T::ResourcePtr &&ptr,
        std::optional<std::reference_wrapper<RenderTaskBase>> task
    ) {
        auto resource_ptr =
          std::make_unique<T>(
            std::move(name),
            std::move(ptr),
            std::move(task)
          );

        resource_ptr->m_id = m_next_id++;
        const auto resource_id = resource_ptr->id();

        m_resources.emplace_back(std::move(resource_ptr));

        return resource_id;
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
