// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "RenderTaskBuilder.hpp"
#include "RenderGraph.hpp"

namespace storm::engine {
	template <typename Resource, typename Description>
	ResourceBase::ID RenderTaskBuilder::create(std::string name, Description &&description, std::vector<std::uint32_t> &create_resources) {
		auto id = 0u;
		if constexpr(std::is_same_v<std::decay_t<Resource>, TextureResource>)
			id = m_pool.addTransientTexture(
						m_device,
						std::move(name),
						std::forward<Description>(description),
						std::ref(m_task)
					);
		else if constexpr(std::is_same_v<std::decay_t<Resource>, UniformBufferResource>)
			id = m_pool.addTransientUniformBuffer(
						m_device,
						std::move(name),
						std::forward<Description>(description),
						std::ref(m_task)
					);
		else static_assert (true, "Invalid Resource type");

		create_resources.emplace_back(id);

		return id;
	}

	template <typename Resource>
	ResourceBase::ID RenderTaskBuilder::write(ResourceBase::ID resourceID, std::vector<std::uint32_t> &write_resources) {
		auto &resource = m_pool.getResource(resourceID);
		resource.m_writers.emplace_back(m_task.id());
		
		write_resources.emplace_back(resourceID);

		return resource.id();
	}

	template <typename Resource>
	ResourceBase::ID RenderTaskBuilder::read(ResourceBase::ID resourceID, std::vector<std::uint32_t> &read_resources) {
		auto &resource = m_pool.getResource(resourceID);
		resource.m_readers.emplace_back(m_task.id());
		
		read_resources.emplace_back(resourceID);

		return resource.id();
	}
}
