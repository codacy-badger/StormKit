// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <variant>
#include <memory>

#include <storm/core/NonCopyable.hpp>

#include <storm/engine/render/Device.hpp>

#include <storm/engine/graphics/ResourceBase.hpp>

namespace storm::engine {
	template <typename ResourceDescription_, typename ResourceType_>
	class Resource : public ResourceBase {
		public:
			using ResourceDescription = ResourceDescription_;
			using ResourceType        = ResourceType_;

			Resource(const Device &device, std::string name,
					 ResourceDescription &&description, RenderTaskBase *creator);
			Resource(std::string name, ResourceDescription &&description, ResourceType &resource);

			Resource(Resource &&);
			Resource &operator=(Resource &&);

			inline const ResourceDescription &description() const noexcept;
			inline const ResourceType &resource() const noexcept;
			inline ResourceType *resourcePtr() const noexcept;
		private:
			Resource(std::string &&name, ResourceDescription &&description);

			using ResourcePtr = std::unique_ptr<ResourceType>;
			using ResourceVariant = std::variant<ResourceType*, ResourcePtr>;

			ResourceDescription m_description;
			ResourceVariant     m_resource;
	};
}

#include "Resource.tpp"
