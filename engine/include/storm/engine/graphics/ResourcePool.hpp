// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <cstdint>
#include <storm/core/NonCopyable.hpp>
#include <storm/engine/graphics/Resource.hpp>
#include <unordered_map>
#include <vector>

namespace storm::engine {
	class ResourcePool : public core::NonCopyable {
	public:
		explicit ResourcePool();
		~ResourcePool();

		ResourcePool(ResourcePool &&);
		ResourcePool &operator=(ResourcePool &&);

        template <typename T>
        ResourceBase::ID addRetainedResource(
          std::string name,
          T &resource
        );

        template <typename T>
        ResourceBase::ID addTransientResource(
          std::string name,
          typename T::ResourcePtr &&ptr,
          std::optional<std::reference_wrapper<RenderTaskBase>> task
        );

		template <typename T>
		const T &acquireResourceAs(ResourceBase::ID resource_id) const noexcept;

		template <typename T>
		T &acquireResourceAs(ResourceBase::ID resource_id) noexcept;

		template <typename T>
		const T *acquireResourcePtrAs(ResourceBase::ID resource_id) const noexcept;

		template <typename T>
		T *acquireResourcePtrAs(ResourceBase::ID resource_id) noexcept;
		
		const ResourceBase &acquireResource(ResourceBase::ID resource_id) const
		    noexcept;
		ResourceBase &acquireResource(ResourceBase::ID resource_id) noexcept;

		void clear();

		inline const std::vector<ResourceBase::Ptr> &resourceArray() const
          noexcept {
			return m_resources;
		}

	private:
		std::vector<ResourceBase::Ptr> m_resources;

		ResourceBase::ID m_next_id;
	};
}

#include "ResourcePool.tpp"
