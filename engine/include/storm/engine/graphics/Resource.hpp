// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <memory>
#include <storm/core/NonCopyable.hpp>
#include <storm/engine/graphics/ResourceBase.hpp>
#include <storm/engine/render/Device.hpp>
#include <storm/engine/render/Framebuffer.hpp>
#include <variant>

namespace storm::engine {
    template <typename T>
	class Resource : public ResourceBase {
    public:
        using ResourceType        = std::decay_t<T>;
        using ResourcePtr     = std::unique_ptr<ResourceType>;

    private:
        using ResourceVariant = std::variant<ResourceType *, ResourcePtr>;

    public:
        SUR_Object(Resource<T>)

        Resource(std::string name,
                 ResourcePtr &&ptr,
                 ResourceBase::RenderTaskBaseOptionalRef creator
        );

        Resource(std::string name, T &resource);

		Resource(Resource &&);
		Resource &operator=(Resource &&);

        inline const ResourceType &resource() const noexcept;
        inline ResourceType &resource() noexcept;
    private:
        void derealize() override;

        ResourceVariant     m_resource;
	};
}

#include "Resource.tpp"
