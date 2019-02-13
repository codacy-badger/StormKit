// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/NonCopyable.hpp>
#include <storm/engine/graphics/ForwardDeclarations.hpp>
#include <storm/engine/graphics/RenderTask.hpp>
#include <storm/engine/graphics/RenderTaskBuilder.hpp>
#include <storm/engine/graphics/Resource.hpp>
#include <storm/engine/graphics/ResourcePool.hpp>
#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/RenderPass.hpp>
#include <storm/engine/render/Texture.hpp>

namespace storm::engine {
	class RenderGraph : public core::NonCopyable {
	public:
		explicit RenderGraph(const Device &device);
		~RenderGraph();

		RenderGraph(RenderGraph &&);
		RenderGraph &operator=(RenderGraph &&);

		template <typename RenderTaskData>
		RenderTask<RenderTaskData> &addRenderPass(const std::string name,
		    typename RenderTask<RenderTaskData>::SetupFunction      setup_func,
		    typename RenderTask<RenderTaskData>::ExecuteFunction execute_func);

		std::uint32_t addRetainedAttachment(std::string name,
		    Texture::Description description, Texture &resource) {
			return m_resources.addRetainedTexture(
			    std::move(name), std::move(description), resource);
		}

		std::uint32_t addRetainedUniformBuffer(std::string name,
		    UniformBuffer::Description description, UniformBuffer &resource) {
			return m_resources.addRetainedUniformBuffer(
			    std::move(name), std::move(description), resource);
		}

		void compile();
		void execute(CommandBuffer &command_buffer);

		void clear();

		void exportGraphviz(const _std::filesystem::path &filepath) const;

		const RenderTaskBase &getRenderTask(
		    RenderTaskBase::ID render_task_id) const noexcept;
		RenderTaskBase &getRenderTask(
		    RenderTaskBase::ID render_task_id) noexcept;

		template <typename T>
		const T &getRenderTaskAs(RenderTaskBase::ID render_task_id) const
		    noexcept;

		template <typename T>
		T &getRenderTaskAs(RenderTaskBase::ID render_task_id) noexcept;

		inline const Device &device() const noexcept { return m_device.get(); }

	private:
		struct Step {
			Step(RenderTaskBase::ID             task_,
			    std::vector<ResourceBase::ID> &&realized_resources_,
			    std::vector<ResourceBase::ID> &&derealized_resources_)
			    : task {task_}, realized_resources {realized_resources_},
			      derealized_resources {derealized_resources_} {}

			RenderTaskBase::ID            task;
			std::vector<ResourceBase::ID> realized_resources;
			std::vector<ResourceBase::ID> derealized_resources;
		};

		std::reference_wrapper<const Device> m_device;

		std::vector<std::unique_ptr<RenderTaskBase>> m_render_tasks;
		std::vector<Step>                            m_timeline;

		ResourcePool m_resources;

		std::uint32_t m_next_task_id;
		friend class RenderTaskBuilder;
	};
}

#include "RenderGraph.tpp"
