// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/Utils.hpp>

#include <storm/core/Pimpl.hpp>
#include <storm/core/Memory.hpp>

namespace storm::engine {
	class UniformBufferImpl;
	class UniformBuffer {
		public:
			Unique_Object(UniformBuffer)

			explicit UniformBuffer(const Device &device, std::size_t size, std::size_t alignement);
			~UniformBuffer();

			UniformBuffer(UniformBuffer &&);
			UniformBuffer &operator=(UniformBuffer &&);

			void reset();

			std::size_t addData(const std::byte *data, std::size_t size);
			void updateData(const std::byte *data, std::size_t size, std::ptrdiff_t offset);

			template<class Container>
			inline std::size_t addData(Container &&container);
			template<class Container>
			inline void updateData(Container &&container, std::ptrdiff_t offset);

			std::byte *map();
			std::byte *map(std::size_t size, std::ptrdiff_t offset);
			void       unmap();

			IMPLEMENTATION(UniformBufferImpl)
		private:
			core::Pimpl<UniformBufferImpl> m_impl;
	};
}

#include "UniformBuffer.tpp"
