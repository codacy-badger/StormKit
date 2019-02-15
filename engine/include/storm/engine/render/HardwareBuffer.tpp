// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "HardwareBuffer.hpp"

namespace storm::engine {
	template <class Container>
	inline void HardwareBuffer::setData(
		Container &&container, std::ptrdiff_t offset) {
		using value_type =
			typename std::remove_reference_t<Container>::value_type;

		const auto container_size = std::size(container);
		const auto container_data =
				reinterpret_cast<const std::byte*>(std::data(container));
		const auto value_type_size = sizeof(value_type);

		return setData(container_data, container_size * value_type_size, offset);
	}
}
