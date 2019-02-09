// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <cstdint>

#include <vector>
#include <unordered_map>

#include <storm/core/NonCopyable.hpp>

#include <storm/engine/graphics/Resource.hpp>

namespace storm::engine {
	class ResourcePool : public core::NonCopyable {
		public:
			explicit ResourcePool();
			~ResourcePool();

			ResourcePool(ResourcePool &&);
			ResourcePool &operator=(ResourcePool &&);
			
			template <typename ResourceType, typename ResourceDescription>
			ResourceBase::ID addRetainedResource(
					std::string name,
					ResourceDescription &&description,
					ResourceType &resource);
			
			template <typename Resource, typename ResourceDescription>
			ResourceBase::ID addTransientResource(
					const Device &device, 
					std::string name,
					ResourceDescription &&description,
					std::optional<std::reference_wrapper<RenderTaskBase>> task);
			
			template <typename T>
			const T &acquireResourceAs(ResourceBase::ID resource_id) const noexcept;

			template <typename T>
			T &acquireResourceAs(ResourceBase::ID resource_id) noexcept;

			const ResourceBase &acquireResource(ResourceBase::ID resource_id) const noexcept;
			ResourceBase &acquireResource(ResourceBase::ID resource_id) noexcept;
			
			void clear();
			
			inline const std::vector<ResourceBase::Ptr> &resourceArray() const noexcept { return m_resources; }
		private:
			std::vector<ResourceBase::Ptr> m_resources;
			
			ResourceBase::ID m_next_id;
	};
}

#include "ResourcePool.tpp"
