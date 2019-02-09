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
	template <typename ResourceDescription, typename ResourceType>
	std::unique_ptr<ResourceType> realize(const Device &device, ResourceDescription);

	template <>
	Texture::Ptr realize(const Device &device, Texture::Description description);

	template <>
	UniformBuffer::Ptr realize(const Device &device, UniformBuffer::Description description);

	template <typename ResourceDescription_, typename ResourceType_>
	class Resource : public ResourceBase {
			using MyType = Resource<ResourceDescription_, ResourceType_>;
		public:
			using ResourceDescription = std::decay_t<ResourceDescription_>;
			using ResourceType        = std::decay_t<ResourceType_>;
			using ResourcePtr         = std::unique_ptr<ResourceType>;

			SUR_Object(MyType)

			template <typename ResourceDescription__>
			Resource(const Device &device, 
					 std::string name,
					 ResourceDescription__ &&description,
					 ResourceBase::RenderTaskBaseOptionalRef creator);

			template <typename ResourceDescription__>
			Resource(std::string name, 
					 ResourceDescription__ &&description,
					 ResourceType_ &resource);

			Resource(Resource &&);
			Resource &operator=(Resource &&);

			inline const ResourceDescription &description() const noexcept;
			inline const ResourceType &resource() const noexcept;
		private:
			using ResourceVariant = std::variant<ResourceType*, ResourcePtr>;

			void realize(const Device &device) override {
				if(transient()) {
					auto ptr = storm::engine::realize<ResourceDescription, ResourceType>(device, m_description);

					m_resource = std::move(ptr);
				}
			}

			void derealize() override {
				if(transient()) std::get<ResourcePtr>(m_resource).reset(nullptr);
			}

			ResourceDescription m_description;
			ResourceVariant     m_resource;
	};

	using TextureResource = Resource<Texture::Description, Texture>;
	using UniformBufferResource = Resource<UniformBuffer::Description, UniformBuffer>;
}

#include "Resource.tpp"
