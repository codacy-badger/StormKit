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
ResourceBase::ID ResourcePool::addRetainedUniformBuffer(std::string name,
    UniformBuffer::Description description, UniformBuffer &resource) {
	m_buffers.emplace_back(UniformBufferResource::makeUnique(
	    std::move(name), std::move(description), std::ref(resource)));

	auto &resource_ = *m_buffers.back();
	resource_.m_id  = m_next_id++;

	return resource_.id();
}

/////////////////////////////////////
/////////////////////////////////////
ResourceBase::ID ResourcePool::addTransientUniformBuffer(const Device &device,
    std::string name, UniformBuffer::Description description,
    std::optional<std::reference_wrapper<RenderTaskBase>> task) {
	m_buffers.emplace_back(UniformBufferResource::makeUnique(
	    device, std::move(name), std::move(description), std::move(task)));

	auto &resource_ = *m_buffers.back();
	resource_.m_id  = m_next_id++;

	return resource_.id();
}

/////////////////////////////////////
/////////////////////////////////////
ResourceBase::ID ResourcePool::addRetainedTexture(
    std::string name, Texture::Description description, Texture &resource) {
	m_textures.emplace_back(TextureResource::makeUnique(
	    std::move(name), std::move(description), std::ref(resource)));

	auto &resource_ = *m_textures.back();
	resource_.m_id  = m_next_id++;

	return resource_.id();
}

/////////////////////////////////////
/////////////////////////////////////
ResourceBase::ID ResourcePool::addTransientTexture(const Device &device,
    std::string name, Texture::Description description,
    std::optional<std::reference_wrapper<RenderTaskBase>> task) {
	m_textures.emplace_back(TextureResource::makeUnique(
	    device, std::move(name), std::move(description), std::move(task)));

	auto &resource_ = *m_textures.back();
	resource_.m_id  = m_next_id++;

	return resource_.id();
}

/////////////////////////////////////
/////////////////////////////////////
const ResourceBase &ResourcePool::getResource(
    ResourceBase::ID resource_id) const noexcept {
	ASSERT(resource_id < m_next_id, "Resource id out of bound");

	const auto check_if_id_are_same = [resource_id](const auto &resource) {
		return resource->id() == resource_id;
	};

	auto texture_it = std::find_if(
	    std::cbegin(m_textures), std::cend(m_textures), check_if_id_are_same);

	if (texture_it != std::cend(m_textures))
		return *texture_it->get();

	auto buffer_it = std::find_if(
	    std::cbegin(m_buffers), std::cend(m_buffers), check_if_id_are_same);

	ASSERT(buffer_it != std::cend(m_buffers),
	    core::format("Failed to find resource id %{1}", resource_id));

	return *buffer_it->get();
}

/////////////////////////////////////
/////////////////////////////////////
ResourceBase &ResourcePool::getResource(ResourceBase::ID resource_id) noexcept {
	ASSERT(resource_id < m_next_id, "Resource id out of bound");

	const auto check_if_id_are_same = [resource_id](const auto &resource) {
		return resource->id() == resource_id;
	};

	auto texture_it = std::find_if(
	    std::begin(m_textures), std::end(m_textures), check_if_id_are_same);

	if (texture_it != std::end(m_textures))
		return *texture_it->get();

	auto buffer_it = std::find_if(
	    std::begin(m_buffers), std::end(m_buffers), check_if_id_are_same);

	ASSERT(buffer_it != std::end(m_buffers),
	    core::format("Failed to find resource id %{1}", resource_id));

	return *buffer_it->get();
}

/////////////////////////////////////
/////////////////////////////////////
const UniformBufferResource &ResourcePool::getBuffer(
    ResourceBase::ID resource_id) const noexcept {
	ASSERT(resource_id < m_next_id, "Resource id out of bound");

	auto it = std::find_if(std::cbegin(m_buffers), std::cend(m_buffers),
	    [resource_id](
	        const auto &resource) { return resource->id() == resource_id; });

	ASSERT(it != std::cend(m_buffers),
	    core::format("Failed to find buffer id %{1}", resource_id));

	return *it->get();
}

/////////////////////////////////////
/////////////////////////////////////
UniformBufferResource &ResourcePool::getBuffer(
    ResourceBase::ID resource_id) noexcept {
	ASSERT(resource_id < m_next_id, "Resource id out of bound");

	auto it = std::find_if(std::begin(m_buffers), std::end(m_buffers),
	    [resource_id](
	        const auto &resource) { return resource->id() == resource_id; });

	ASSERT(it != std::end(m_buffers),
	    core::format("Failed to find buffer id %{1}", resource_id));

	return *it->get();
}

/////////////////////////////////////
/////////////////////////////////////
const TextureResource &ResourcePool::getTexture(
    ResourceBase::ID resource_id) const noexcept {
	ASSERT(resource_id < m_next_id, "Resource id out of bound");

	auto it = std::find_if(std::cbegin(m_textures), std::cend(m_textures),
	    [resource_id](
	        const auto &resource) { return resource->id() == resource_id; });

	ASSERT(it != std::cend(m_textures),
	    core::format("Failed to find texture id %{1}", resource_id));

	return *it->get();
}

/////////////////////////////////////
/////////////////////////////////////
TextureResource &ResourcePool::getTexture(
    ResourceBase::ID resource_id) noexcept {
	ASSERT(resource_id < m_next_id, "Resource id out of bound");

	auto it = std::find_if(std::begin(m_textures), std::end(m_textures),
	    [resource_id](
	        const auto &resource) { return resource->id() == resource_id; });

	ASSERT(it != std::end(m_textures),
	    core::format("Failed to find texture id %{1}", resource_id));

	return *it->get();
}

/////////////////////////////////////
/////////////////////////////////////
void ResourcePool::clear() {
	m_next_id = 1;

	m_textures.clear();
	m_buffers.clear();
}
