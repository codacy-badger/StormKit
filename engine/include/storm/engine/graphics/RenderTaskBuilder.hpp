// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <utility>
#include <string>

#include <storm/core/NonCopyable.hpp>

namespace storm::engine {
	class Device;
	class RenderTaskBase;
	class RenderGraph;
	class RenderTaskBuilder : public core::NonCopyable {
		public:
			explicit RenderTaskBuilder(const Device &device, RenderTaskBase &task, RenderGraph &graph);
			~RenderTaskBuilder();

			RenderTaskBuilder(RenderTaskBuilder &&);

			template <typename Resource, typename ResourceDescription>
			Resource *create(std::string name, ResourceDescription &&description);

			template <typename Resource>
			Resource *write(Resource &resource);

			template <typename Resource>
			Resource *read(Resource &resource);
		private:
			const Device   &m_device;
			RenderTaskBase &m_task;
			RenderGraph    &m_graph;
	};
}
