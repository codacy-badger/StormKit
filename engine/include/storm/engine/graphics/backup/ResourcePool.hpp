// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <cstdint>
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

		ResourceBase::ID addRetainedUniformBuffer(std::string name,
		    UniformBuffer::Description description, UniformBuffer &resource);

		ResourceBase::ID addTransientUniformBuffer(const Device &device,
		    std::string name, UniformBuffer::Description description,
		    std::optional<std::reference_wrapper<RenderTaskBase>> task);

		ResourceBase::ID addRetainedTexture(std::string name,
		    Texture::Description description, Texture &resource);

		ResourceBase::ID addTransientTexture(const Device &device,
		    std::string name, Texture::Description description,
		    std::optional<std::reference_wrapper<RenderTaskBase>> task);

		const ResourceBase &getResource(ResourceBase::ID resource_id) const
		    noexcept;
		ResourceBase &getResource(ResourceBase::ID resource_id) noexcept;
		const UniformBufferResource &getBuffer(
		    ResourceBase::ID resource_id) const noexcept;
		UniformBufferResource &getBuffer(ResourceBase::ID resource_id) noexcept;
		const TextureResource &getTexture(ResourceBase::ID resource_id) const
		    noexcept;
		TextureResource &getTexture(ResourceBase::ID resource_id) noexcept;

		void clear();

		inline const std::vector<TextureResource::Ptr> &textures() const
		    noexcept;
		inline const std::vector<UniformBufferResource::Ptr> &buffers() const
		    noexcept;

	private:
		std::vector<TextureResource::Ptr>       m_textures;
		std::vector<UniformBufferResource::Ptr> m_buffers;

		ResourceBase::ID m_next_id;
	};
}

#include "ResourcePool.inl"
