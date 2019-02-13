// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/core/Assert.hpp>
#include <storm/core/Strings.hpp>
#include <storm/engine/graphics/ResourcePool.hpp>

using namespace storm;
using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
ResourcePool::ResourcePool() : m_next_id {1} {}

/////////////////////////////////////
/////////////////////////////////////
ResourcePool::~ResourcePool() = default;

/////////////////////////////////////
/////////////////////////////////////
ResourcePool::ResourcePool(ResourcePool &&) = default;

/////////////////////////////////////
/////////////////////////////////////
ResourcePool &ResourcePool::operator=(ResourcePool &&) = default;

/////////////////////////////////////
/////////////////////////////////////
const ResourceBase &ResourcePool::acquireResource(
    ResourceBase::ID resource_id) const noexcept {
	ASSERT(resource_id < m_next_id, "Resource id out of bound");

	auto it = std::find_if(std::begin(m_resources), std::end(m_resources),
	    [resource_id](
	        const auto &resource) { return resource->id() == resource_id; });

	ASSERT(it != std::end(m_resources),
	    core::format("Failed to find resource id %{1}", resource_id));

	return *it->get();
}

/////////////////////////////////////
/////////////////////////////////////
ResourceBase &ResourcePool::acquireResource(
    ResourceBase::ID resource_id) noexcept {
	ASSERT(resource_id < m_next_id, "Resource id out of bound");

	auto it = std::find_if(std::begin(m_resources), std::end(m_resources),
	    [resource_id](
	        const auto &resource) { return resource->id() == resource_id; });

	ASSERT(it != std::end(m_resources),
	    core::format("Failed to find resource id %{1}", resource_id));

	return *it->get();
}

/////////////////////////////////////
/////////////////////////////////////
void ResourcePool::clear() {
	m_next_id = 1;

	m_resources.clear();
}
