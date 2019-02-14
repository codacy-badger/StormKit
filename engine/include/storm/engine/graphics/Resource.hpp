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
	template <typename ResourceDescription_, typename ResourceType_>
	class Resource : public ResourceBase {
		using MyType = Resource<ResourceDescription_, ResourceType_>;

	public:
		using ResourceDescription = std::decay_t<ResourceDescription_>;
		using ResourceType        = std::decay_t<ResourceType_>;

		SUR_Object(MyType)

		    template <typename ResourceDescription__>
		    Resource(const Device &device, std::string name,
		        ResourceDescription__ &&                description,
		        ResourceBase::RenderTaskBaseOptionalRef creator);

		template <typename ResourceDescription__>
		Resource(std::string name, ResourceDescription__ &&description,
		    ResourceType_ &resource);

		Resource(Resource &&);
		Resource &operator=(Resource &&);

		inline const ResourceDescription &description() const noexcept;

	private:
		using ResourcePtr     = std::unique_ptr<ResourceType>;
		using ResourceVariant = std::variant<ResourceType *, ResourcePtr>;

		void realize(const Device &device) override {
		}

		void derealize() override {
			if (transient())
				std::get<ResourcePtr>(m_resource).reset(nullptr);
		}

		ResourceDescription m_description;
		ResourceVariant     m_resource;
	};
}

#include "Resource.tpp"
