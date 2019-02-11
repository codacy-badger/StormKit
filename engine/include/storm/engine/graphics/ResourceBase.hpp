// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <string>
#include <vector>
#include <optional>

#include <storm/core/NonCopyable.hpp>
#include <storm/core/Memory.hpp>

#include <storm/engine/render/ForwardDeclarations.hpp>

#include <storm/engine/graphics/RenderTaskBase.hpp>

namespace storm::engine {
	class ResourceBase : public core::NonCopyable {
		public:
			SUR_Object(ResourceBase)
			
			using ID = std::uint32_t;
			
			using RenderTaskBaseOptionalRef = std::optional<std::reference_wrapper<RenderTaskBase>>;
			
			explicit ResourceBase(std::string name, RenderTaskBaseOptionalRef &&creator);
			virtual ~ResourceBase();

			ResourceBase(ResourceBase &&);
			ResourceBase &operator=(ResourceBase &&);

			inline ID id() const noexcept;
			inline bool transient() const noexcept;

			inline void setName(std::string name);
			inline const std::string &name() const noexcept;
		protected:
			virtual void realize(const Device &device) = 0;
			virtual void derealize() = 0;

			std::string     m_name;

			RenderTaskBaseOptionalRef m_creator;

			std::vector<RenderTaskBase::ID> m_readers;
			std::vector<RenderTaskBase::ID> m_writers;

			ID m_id;
			std::uint32_t m_ref_count;

			friend class RenderTaskBuilder;
			friend class RenderGraph;
			friend class ResourcePool;
	};
}

#include "ResourceBase.inl"
