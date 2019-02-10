// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <string>
#include <set>

#include <storm/core/NonCopyable.hpp>

#include <storm/engine/graphics/ForwardDeclarations.hpp>

namespace storm::engine {
	class RenderTaskBase : core::NonCopyable {
		public:
			using ID = std::uint32_t;
			
			explicit RenderTaskBase(std::string name);
			virtual ~RenderTaskBase();

			RenderTaskBase(RenderTaskBase &&);
			RenderTaskBase &operator=(RenderTaskBase &&);

			inline void setCullImune(bool imune);
			inline bool cullImune() const noexcept;

			inline ID id() const noexcept;
			inline void setName(std::string name);
			inline const std::string &name() const noexcept;
		private:
			virtual void setup(RenderTaskBuilder &builder) = 0;
			virtual void execute() const = 0;

			std::string m_name;
			bool m_cull_imune;

			std::set<std::uint32_t> m_create_resources;
			std::set<std::uint32_t> m_write_resources;
			std::set<std::uint32_t> m_read_resources;

			ID m_id;
			std::uint32_t m_ref_count;

			friend class RenderTaskBuilder;
			friend class RenderGraph;
	};
}

#include "RenderTaskBase.inl"
