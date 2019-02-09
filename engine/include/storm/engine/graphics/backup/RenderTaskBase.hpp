// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <string>
#include <vector>
#include <memory>

#include <storm/core/NonCopyable.hpp>

#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/PipelineState.hpp>

#include <storm/engine/graphics/ForwardDeclarations.hpp>

namespace storm::engine {
	class RenderTaskBase : core::NonCopyable {
		public:
			using ID = std::uint32_t;
			
			explicit RenderTaskBase(std::string name, RenderGraph &parent);
			virtual ~RenderTaskBase();

			RenderTaskBase(RenderTaskBase &&);
			RenderTaskBase &operator=(RenderTaskBase &&);

			inline ID id() const noexcept;
			inline void setName(std::string name);
			inline const std::string &name() const noexcept;
			inline const Framebuffer &framebuffer() const noexcept;
			inline PipelineState &pipelineState() noexcept;

		protected:
			std::unique_ptr<RenderPass>  m_render_pass;
			std::unique_ptr<Framebuffer> m_framebuffer;

		private:
			void initRenderPass(ResourcePool &pool);
			uvec2 computeSize(const std::optional<std::reference_wrapper<const ResourceBase> > &depth, const std::vector<std::reference_wrapper<const ResourceBase> > &colors);

			virtual void setup(RenderTaskBuilder &) = 0;
			virtual void execute(CommandBuffer &) const = 0;

			std::string m_name;
			std::reference_wrapper<RenderGraph> m_parent;

			PipelineState m_pipeline_state;

			std::vector<std::uint32_t> m_create_attachments;
			std::vector<std::uint32_t> m_write_attachments;
			std::vector<std::uint32_t> m_read_attachments;

			std::vector<std::uint32_t> m_create_buffers;
			std::vector<std::uint32_t> m_write_buffers;
			std::vector<std::uint32_t> m_read_buffers;

			ID m_id;
			std::uint32_t m_ref_count;

			friend class RenderTaskBuilder;
			friend class RenderGraph;
	};
}

#include "RenderTaskBase.inl"
