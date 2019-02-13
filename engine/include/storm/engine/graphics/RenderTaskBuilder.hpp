// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/NonCopyable.hpp>
#include <storm/engine/graphics/ForwardDeclarations.hpp>
#include <storm/engine/graphics/RenderTaskBase.hpp>
#include <storm/engine/graphics/ResourceBase.hpp>
#include <storm/engine/render/ForwardDeclarations.hpp>
#include <string>
#include <utility>

namespace storm::engine {
	class RenderTaskBuilder : public core::NonCopyable {
	public:
		explicit RenderTaskBuilder(
		    const Device &device, RenderTaskBase &task, ResourcePool &pool);
		~RenderTaskBuilder();

		RenderTaskBuilder(RenderTaskBuilder &&);

		template <typename Resource, typename ResourceDescription>
		ResourceBase::ID create(
		    std::string name, ResourceDescription &&description);

		template <typename Resource>
		ResourceBase::ID write(ResourceBase::ID resource);

		template <typename Resource>
		ResourceBase::ID read(ResourceBase::ID resource);

	private:
		const Device &  m_device;
		RenderTaskBase &m_task;
		ResourcePool &  m_pool;
	};
}
