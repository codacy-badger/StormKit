// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/Memory.hpp>
#include <storm/core/Pimpl.hpp>
#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/Utils.hpp>

namespace storm::engine {
	class VertexBufferImpl;
	class VertexBuffer {
	public:
		Unique_Object(VertexBuffer)

		    explicit VertexBuffer(
		        const Device &device, std::size_t size, std::size_t alignement);
		~VertexBuffer();

		VertexBuffer(VertexBuffer &&);
		VertexBuffer &operator=(VertexBuffer &&);

		void reset();

		std::size_t addData(const std::byte *data, std::size_t size);
		void        updateData(
		           const std::byte *data, std::size_t size, std::ptrdiff_t offset);

		template <class Container>
		inline std::size_t addData(Container &&container);
		template <class Container>
		inline void updateData(Container &&container, std::ptrdiff_t offset);

		std::byte *map();
		std::byte *map(std::size_t size, std::ptrdiff_t offset);
		void       unmap();

		IMPLEMENTATION(VertexBufferImpl)
	private:
		core::Pimpl<VertexBufferImpl> m_impl;
	};
}

#include "VertexBuffer.tpp"
