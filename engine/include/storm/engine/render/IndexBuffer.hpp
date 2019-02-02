// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/Utils.hpp>

#include <storm/core/Pimpl.hpp>
#include <storm/core/Memory.hpp>

namespace storm::engine {
	class IndexBufferImpl;
	class IndexBuffer {
		public:
			Unique_Object(IndexBuffer)

			explicit IndexBuffer(const Device &device, std::size_t size, std::size_t alignement);
			~IndexBuffer();

			IndexBuffer(IndexBuffer &&);
			IndexBuffer &operator=(IndexBuffer &&);

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

			IMPLEMENTATION(IndexBufferImpl)
		private:
			core::Pimpl<IndexBufferImpl> m_impl;
	};
}

#include "IndexBuffer.tpp"
