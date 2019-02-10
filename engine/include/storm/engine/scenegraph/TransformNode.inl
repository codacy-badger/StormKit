// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include "TransformNode.hpp"

namespace storm::engine {
	inline const TransformNode::DataType &TransformNode::data()      const noexcept {
		return m_transform;
	}

	inline void TransformNode::setTransform(Transform transform) noexcept {
		m_transform = std::move(transform);
		notify(NodeEvent::UPDATED);
	}
}
