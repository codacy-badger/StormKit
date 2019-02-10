// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/graphics/ForwardDeclarations.hpp>
#include <storm/engine/graphics/RenderTaskBase.hpp>
#include <storm/engine/graphics/Resource.hpp>

namespace storm::engine {
	class RenderTaskBuilder;
	template <typename RenderTaskData>
	class RenderTask : public RenderTaskBase {
		public:
			using SetupFunction = std::function<void(RenderTaskData &, RenderTaskBuilder &)>;
			using ExecuteFunction = std::function<void(CommandBuffer &, const RenderTaskData &, RenderPass &, Framebuffer &)>;

			explicit RenderTask(std::string name, RenderGraph &parent, SetupFunction setup_func, ExecuteFunction execute_func);
			virtual ~RenderTask();

			RenderTask(RenderTask &&);
			RenderTask &operator=(RenderTask &&);

			inline const RenderTaskData &data() const noexcept;
		private:
			void setup(RenderTaskBuilder &builder) override;
			void execute(CommandBuffer &command_buffer) const override;

			SetupFunction m_setup_func;
			ExecuteFunction m_execute_func;

			RenderTaskData m_data;
	};
}

#include "RenderTask.tpp"
