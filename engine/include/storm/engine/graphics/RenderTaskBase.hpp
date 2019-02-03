// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <string>
#include <vector>

#include <storm/core/NonCopyable.hpp>

namespace storm::engine {
	class ResourceBase;
	class RenderTaskBuilder;
	class RenderTaskBase : core::NonCopyable {
		public:
			explicit RenderTaskBase(std::string name);
			virtual ~RenderTaskBase();

			RenderTaskBase(RenderTaskBase &&);
			RenderTaskBase &operator=(RenderTaskBase &&);

			inline void setName(std::string name);
			inline const std::string &name() const noexcept;
		private:
			virtual void setup(RenderTaskBuilder &builder) = 0;
			virtual void execute() const = 0;

			std::string m_name;

			std::vector<const ResourceBase *> m_create_resources;
			std::vector<const ResourceBase *> m_write_resources;
			std::vector<const ResourceBase *> m_read_resources;

			std::uint32_t m_ref_count;

			friend class RenderTaskBuilder;
			friend class RenderGraph;
	};
}

#include "RenderTaskBase.inl"
