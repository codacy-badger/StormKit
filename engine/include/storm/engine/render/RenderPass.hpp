// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/Memory.hpp>
#include <storm/core/NonCopyable.hpp>
#include <storm/core/Pimpl.hpp>
#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/Types.hpp>
#include <storm/engine/render/Utils.hpp>
#include <vector>

namespace storm::engine {
	class RenderPassImpl;
	class RenderPass : public core::NonCopyable {
	public:
		Unique_Object(RenderPass)

		    struct SubPass {
			static constexpr const auto EXTERNAL
			    = std::numeric_limits<std::uint32_t>::max();

			std::vector<std::uint32_t> previous_subpass;
			std::vector<std::size_t>   input_attachments;
			std::vector<std::size_t>   output_attachments;
		};

		explicit RenderPass(const Device &device);
		~RenderPass();

		RenderPass(RenderPass &&);

		std::size_t addSubPass(SubPass subpass);
		std::size_t addAttachment(ColorFormat format);

		bool hasDepthAttachment() const noexcept;

		void setExtent(uvec2 extent);

		void build();

		IMPLEMENTATION(RenderPassImpl)
	private:
		core::Pimpl<RenderPassImpl> m_impl;
	};
}
