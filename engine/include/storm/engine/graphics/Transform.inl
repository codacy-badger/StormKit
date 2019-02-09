// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/graphics/Transform.hpp>

namespace storm::engine {
	inline void Transform::setPosition(const vec3 &position) noexcept {
		m_position = position;
		m_recompute = true;
	}

	inline void Transform::setPosition(vec3 &&position) noexcept {
		m_position = std::move(position);
		m_recompute = true;
	}

	inline void Transform::setOrigin(const vec3 &origin) noexcept {
		m_origin = origin;
		m_recompute = true;
	}

	inline void Transform::setOrigin(vec3 &&origin) noexcept {
		m_origin = std::move(origin);
		m_recompute = true;
	}

	inline void Transform::move(const vec3 &position) noexcept {
		m_position += position;
		m_recompute = true;
	}

	inline void Transform::move(vec3 &&position) noexcept {
		m_position += std::move(position);
		m_recompute = true;
	}

	inline void Transform::setX(float x) noexcept {
		m_position.x = x;
		m_recompute = true;
	}

	inline void Transform::setY(float y) noexcept {
		m_position.y = y;
		m_recompute = true;
	}

	inline void Transform::setZ(float z) noexcept {
		m_position.z = z;
		m_recompute = true;
	}
	inline void Transform::moveX(float x) noexcept {
		m_position.x += x;
		m_recompute = true;
	}

	inline void Transform::moveY(float y) noexcept {
		m_position.y += y;
		m_recompute = true;
	}

	inline void Transform::moveZ(float z) noexcept {
		m_position.z += z;
		m_recompute = true;
	}

	inline void Transform::setScale(float x, float y, float z) noexcept {
		m_scale = {x, y, z};
		m_recompute = true;
	}

	inline void Transform::setScale(const vec3 &scale) noexcept {
		m_scale = scale;
		m_recompute = true;
	}

	inline void Transform::scale(float x, float y, float z) noexcept {
		m_scale += vec3{x, y, z};
		m_recompute = true;
	}

	inline void Transform::scale(const vec3 &scale) noexcept {
		m_scale += scale;
		m_recompute = true;
	}

	inline void Transform::scaleX(float x) noexcept {
		m_scale.x += x;
		m_recompute = true;
	}

	inline void Transform::scaleY(float y) noexcept {
		m_scale.y += y;
		m_recompute = true;
	}

	inline void Transform::scaleZ(float z) noexcept {
		m_scale.z += z;
		m_recompute = true;
	}

	inline void Transform::setPitch(float pitch) noexcept {
		m_pitch = pitch;
		m_recompute = true;
	}

	inline void Transform::setYaw(float yaw) noexcept {
		m_yaw = yaw;
		m_recompute = true;
	}

	inline void Transform::setRoll(float roll) noexcept {
		m_roll = roll;
		m_recompute = true;
	}

	inline const vec3 &Transform::position() const noexcept {
		return m_position;
	}

	inline const vec3 &Transform::origin() const noexcept {
		return m_origin;
	}

	inline const vec3 &Transform::scale() const noexcept {
		return m_scale;
	}

	inline float Transform::pitch() const noexcept {
		return m_pitch;
	}

	inline float Transform::yaw()   const noexcept {
		return m_yaw;
	}

	inline float Transform::roll()  const noexcept {
		return m_roll;
	}

	inline mat4 Transform::matrix() const noexcept {
		if(m_recompute) {
			recompute();
			m_recompute = false;
		}

		return m_transform;
	}
	inline Transform::operator mat4() const noexcept {
		return matrix();
	}

	inline void Transform::forceRecompute() const noexcept {
		recompute();
		m_recompute = false;
	}
}
