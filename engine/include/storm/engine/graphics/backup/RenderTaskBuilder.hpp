// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <utility>
#include <string>

#include <storm/core/NonCopyable.hpp>

#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/Texture.hpp>
#include <storm/engine/render/UniformBuffer.hpp>

#include <storm/engine/graphics/ForwardDeclarations.hpp>
#include <storm/engine/graphics/ResourceBase.hpp>

namespace storm::engine {
	class RenderTaskBuilder : public core::NonCopyable {
		public:
			explicit RenderTaskBuilder(const Device &device, RenderTaskBase &task, ResourcePool &pool);
			~RenderTaskBuilder();

			RenderTaskBuilder(RenderTaskBuilder &&);

			ResourceBase::ID createAttachment(std::string name, Texture::Description description);
			ResourceBase::ID writeAttachment(ResourceBase::ID attachment);
			ResourceBase::ID readAttachment(ResourceBase::ID attachment);

			ResourceBase::ID createUniformBuffer(std::string name, UniformBuffer::Description description);
			ResourceBase::ID writeUniformBuffer(ResourceBase::ID buffer);
			ResourceBase::ID readUniformBuffer(ResourceBase::ID buffer);
		private:
			template <typename Resource, typename Description>
			ResourceBase::ID create(std::string name, Description &&description, std::vector<std::uint32_t> &create_resources);

			template <typename Resource>
			ResourceBase::ID write(ResourceBase::ID attachment, std::vector<std::uint32_t> &write_resources);

			template <typename Resource>
			ResourceBase::ID read(ResourceBase::ID attachment, std::vector<std::uint32_t> &read_resources);

			const Device   &m_device;
			RenderTaskBase &m_task;
			ResourcePool   &m_pool;
	};
}

#include "RenderTaskBuilder.tpp"
