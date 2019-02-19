// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "Resource.hpp"

namespace storm::engine {
    template <typename T>
    Resource<T>::Resource(
        std::string name,
        ResourcePtr &&ptr,
	    ResourceBase::RenderTaskBaseOptionalRef creator)
        : ResourceBase {std::move(name), std::move(creator)}
          {
        m_resource = std::move(ptr);
	}

    template <typename T>
    Resource<T>::Resource(std::string name, T&resource)
        : ResourceBase {std::move(name), std::nullopt}
          {
		m_resource = &resource;
	}

    template <typename T>
    Resource<T>::Resource(Resource &&) = default;

    template <typename T>
    Resource<T>& Resource<T>::operator=(Resource &&) = default;

    template <typename T>
    void Resource<T>::derealize() {
       if (transient())
          std::get<ResourcePtr>(m_resource).reset(nullptr);
    }

    template <typename T>
    inline const typename Resource<T>::ResourceType &
    Resource<T>::resource() const noexcept {
        if (std::holds_alternative<ResourcePtr>(m_resource))
            return *std::get<ResourcePtr>(m_resource);

        return *std::get<ResourceType *>(m_resource);
    }

    template <typename T>
    inline typename Resource<T>::ResourceType &
    Resource<T>::resource() noexcept {
        if (std::holds_alternative<ResourcePtr>(m_resource))
            return *std::get<ResourcePtr>(m_resource);

        return *std::get<ResourceType *>(m_resource);
    }
}
