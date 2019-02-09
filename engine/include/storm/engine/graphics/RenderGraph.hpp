// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/NonCopyable.hpp>

#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/RenderPass.hpp>
#include <storm/engine/render/Texture.hpp>

#include <storm/engine/graphics/ForwardDeclarations.hpp>
#include <storm/engine/graphics/Resource.hpp>
#include <storm/engine/graphics/ResourcePool.hpp>
#include <storm/engine/graphics/RenderTaskBuilder.hpp>
#include <storm/engine/graphics/RenderTask.hpp>

namespace storm::engine {
	using TextureResource = Resource<Texture::Description, Texture>;
	using UniformBufferResource = Resource<UniformBuffer::Description, UniformBuffer>;
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

			template <typename ResourceType, typename ResourceDescription>
			std::uint32_t addRetainedResource(std::string name,
									 ResourceDescription &&description,
									 ResourceType &resource);

			void compile();
			void execute();

			void clear();

			void exportGraphviz(const _std::filesystem::path &filepath) const;
			
			const RenderTaskBase &getRenderTask(RenderTaskBase::ID render_task_id) const noexcept;
			RenderTaskBase &getRenderTask(RenderTaskBase::ID render_task_id) noexcept;
			
			template <typename T>
			const T &getRenderTaskAs(RenderTaskBase::ID render_task_id) const noexcept;
			
			template <typename T>
			T &getRenderTaskAs(RenderTaskBase::ID render_task_id) noexcept;

			inline const ResourcePool &pool() const noexcept { return m_resources; }
		private:
			std::reference_wrapper<const Device> m_device;

			std::vector<std::unique_ptr<RenderTaskBase>> m_render_tasks;
			
			ResourcePool m_resources;

			std::uint32_t m_next_task_id;
			friend class RenderTaskBuilder;
	};
}

#include "RenderTaskBuilder.tpp"
#include "RenderGraph.tpp"
