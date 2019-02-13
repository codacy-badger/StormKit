// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "Resource.hpp"

namespace storm::engine {
	template <typename ResourceDescription_, typename ResourceType_>
	template <typename ResourceDescription__>
	Resource<ResourceDescription_, ResourceType_>::Resource(
	    const Device &device, std::string name,
	    ResourceDescription__ &&                description,
	    ResourceBase::RenderTaskBaseOptionalRef creator)
	    : ResourceBase {std::move(name), std::move(creator)},
	      m_description {std::forward<ResourceDescription__>(description)} {
		m_resource = std::make_unique<ResourceType>(device, m_description);
	}

	template <typename ResourceDescription_, typename ResourceType_>
	template <typename ResourceDescription__>
	Resource<ResourceDescription_, ResourceType_>::Resource(std::string name,
	    ResourceDescription__ &&description, ResourceType_ &resource)
	    : ResourceBase {std::move(name), std::nullopt},
	      m_description {std::forward<ResourceDescription__>(description)} {
		m_resource = &resource;
	}

	template <typename ResourceDescription_, typename ResourceType_>
	Resource<ResourceDescription_, ResourceType_>::Resource(Resource &&)
	    = default;

	template <typename ResourceDescription_, typename ResourceType_>
	Resource<ResourceDescription_, ResourceType_> &
	Resource<ResourceDescription_, ResourceType_>::operator=(Resource &&)
	    = default;

	template <typename ResourceDescription_, typename ResourceType_>
	inline const typename Resource<ResourceDescription_,
	    ResourceType_>::ResourceDescription &
	Resource<ResourceDescription_, ResourceType_>::description() const
	    noexcept {
		return m_description;
	}

	template <typename ResourceDescription_, typename ResourceType_>
	inline const typename Resource<ResourceDescription_,
	    ResourceType_>::ResourceType &
	Resource<ResourceDescription_, ResourceType_>::resource() const noexcept {
		if (std::holds_alternative<ResourcePtr>(m_resource))
			return *std::get<ResourcePtr>(m_resource);

		return *std::get<ResourceType *>(m_resource);
	}

	template <typename ResourceDescription_, typename ResourceType_>
	inline
	    typename Resource<ResourceDescription_, ResourceType_>::ResourceType &
	    Resource<ResourceDescription_, ResourceType_>::resource() noexcept {
		if (std::holds_alternative<ResourcePtr>(m_resource))
			return *std::get<ResourcePtr>(m_resource);

		return *std::get<ResourceType *>(m_resource);
	}
}
