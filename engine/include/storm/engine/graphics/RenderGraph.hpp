// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/NonCopyable.hpp>

#include <storm/engine/render/RenderPass.hpp>
#include <storm/engine/render/Texture.hpp>

#include <storm/engine/graphics/Resource.hpp>
#include <storm/engine/graphics/RenderTask.hpp>
#include <storm/engine/graphics/RenderTaskBuilder.hpp>

namespace storm::engine {
	using TextureResource = Resource<TextureDescription, Texture>;
	class RenderGraph : core::NonCopyable {
		public:
			explicit RenderGraph(const Device &device);
			~RenderGraph();

			RenderGraph(RenderGraph &&);
			RenderGraph &operator=(RenderGraph &&);

			template <typename RenderTaskData>
			RenderTask<RenderTaskData> &addRenderPass(const std::string name,
													  typename RenderTask<RenderTaskData>::SetupFunction setup_func,
													  typename RenderTask<RenderTaskData>::ExecuteFunction execute_func);
			void removeRenderPass(const std::string name);

			template <typename ResourceDescription, typename ResourceType>
			Resource<std::remove_reference_t<ResourceDescription>, std::remove_reference_t<ResourceType>>
				*addRetainedResource(std::string name,
									 ResourceDescription &&description,
									 ResourceType *resource);

			void compile();
			void execute();

			void clear();

			void exportGraphviz(const _std::filesystem::path &filepath) const;
		private:
			std::reference_wrapper<const Device> m_device;

			std::vector<std::unique_ptr<RenderTaskBase>> m_render_tasks;

			std::vector<std::unique_ptr<ResourceBase>> m_resources;

			friend class RenderTaskBuilder;
	};
}

#include "RenderTaskBuilder.tpp"
#include "RenderGraph.tpp"
