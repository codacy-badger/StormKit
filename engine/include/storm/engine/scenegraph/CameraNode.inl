// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include "CameraNode.hpp"

namespace storm::engine {
	inline const mat4 &CameraNode::view() const noexcept {
		return m_camera.view;
	}

	inline void CameraNode::setView(mat4 view) noexcept {
		m_camera.view          = std::move(view);
		m_camera.is_view_dirty = true;
	}

	inline const mat4 &CameraNode::projection() const noexcept {
		return m_camera.projection;
	}

	inline void CameraNode::setProjection(mat4 projection) noexcept {
		m_camera.projection          = std::move(projection);
		m_camera.is_projection_dirty = true;
	}

	inline const CameraNode::DataType &CameraNode::data() const noexcept {
		return m_camera;
	}
}
