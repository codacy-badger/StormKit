// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/render/IndexBuffer.hpp>

namespace storm::engine {
	template<class Container>
	inline std::size_t IndexBuffer::addData(Container &&container) {
		return addData(reinterpret_cast<const std::byte*>(std::data(container)), std::size(container) * sizeof(typename std::remove_reference_t<Container>::value_type));
	}

	template<class Container>
	inline void IndexBuffer::updateData(Container &&container, std::ptrdiff_t offset) {
		updateData(reinterpret_cast<const std::byte*>(std::data(container)), std::size(container) * sizeof(typename std::remove_reference_t<Container>::value_type), offset);
	}
}
