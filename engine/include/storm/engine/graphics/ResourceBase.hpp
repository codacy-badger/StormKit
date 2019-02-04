// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <string>
#include <set>

#include <storm/core/NonCopyable.hpp>

namespace storm::engine {
	class RenderTaskBase;
	class ResourceBase : public core::NonCopyable {
		public:
			explicit ResourceBase(std::string name, RenderTaskBase *creator);
			virtual ~ResourceBase();

			ResourceBase(ResourceBase &&);
			ResourceBase &operator=(ResourceBase &&);

			inline bool transient() const noexcept;

			inline void setName(std::string name);
			inline const std::string &name() const noexcept;
		protected:
			std::string     m_name;

			RenderTaskBase *m_creator;

			std::set<const RenderTaskBase *> m_readers;
			std::set<const RenderTaskBase *> m_writers;

			std::uint32_t m_ref_count;

			friend class RenderTaskBuilder;
			friend class RenderGraph;
	};
}

#include "ResourceBase.inl"
