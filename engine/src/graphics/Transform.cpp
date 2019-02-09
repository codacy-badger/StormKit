#include <storm/engine/graphics/Transform.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
Transform::Transform() noexcept
	: m_recompute(true), m_position(0.f, 0.f, 0.f), m_scale(1.f, 1.f, 1.f), m_pitch(0.f), m_yaw(0.f), m_roll(0.f), m_transform(1.f) { }

/////////////////////////////////////
/////////////////////////////////////
Transform::~Transform() noexcept = default;

/////////////////////////////////////
/////////////////////////////////////
Transform::Transform(const Transform &) noexcept = default;

/////////////////////////////////////
/////////////////////////////////////
Transform &Transform::operator=(const Transform &) noexcept = default;

/////////////////////////////////////
/////////////////////////////////////
Transform::Transform(Transform &&) noexcept = default;

/////////////////////////////////////
/////////////////////////////////////
Transform &Transform::operator=(Transform &&) noexcept = default;

/////////////////////////////////////
/////////////////////////////////////
void Transform::operator*=(const Transform &transform) noexcept {
	if(m_recompute) {
		recompute();
		m_recompute = false;
	}

	m_transform *= transform.matrix();
}

/////////////////////////////////////
/////////////////////////////////////
void Transform::operator+=(const Transform &transform) noexcept {
	if(m_recompute) {
		recompute();
		m_recompute = false;
	}

	m_transform += transform.matrix();
}

/////////////////////////////////////
/////////////////////////////////////
void Transform::operator/=(const Transform &transform) noexcept {
	if(m_recompute) {
		recompute();
		m_recompute = false;
	}

	m_transform /= transform.matrix();
}

/////////////////////////////////////
/////////////////////////////////////
void Transform::operator-=(const Transform &transform) noexcept {
	if(m_recompute) {
		recompute();
		m_recompute = false;
	}

	m_transform -= transform.matrix();
}

/////////////////////////////////////
/////////////////////////////////////
void Transform::recompute() const noexcept {
	m_quat = glm::quat(glm::vec3(glm::radians(m_pitch), glm::radians(m_yaw), glm::radians(m_roll)));
	m_quat = glm::normalize(m_quat);

	auto scale       = glm::scale(glm::mat4(1.f), m_scale);
	auto inv_pivot   = glm::translate(glm::mat4(1.f), -m_origin);
	auto rotation    = glm::mat4_cast(m_quat);
	auto pivot       = glm::translate(glm::mat4(1.f), m_origin);
	auto translation = glm::translate(glm::mat4(1.f), m_position);

	m_transform =  translation * pivot * scale * rotation * inv_pivot;
}

/////////////////////////////////////
/////////////////////////////////////
Transform operator*(const Transform &a, const Transform &b) noexcept {
	Transform c = a;
	c *= b;
	return c;
}

/////////////////////////////////////
/////////////////////////////////////
Transform operator+(const Transform &a, const Transform &b) noexcept {
	Transform c = a;
	c += b;
	return c;
}

/////////////////////////////////////
/////////////////////////////////////
Transform operator/(const Transform &a, const Transform &b) noexcept {
	Transform c = a;
	c /= b;
	return c;
}

/////////////////////////////////////
/////////////////////////////////////
Transform operator-(const Transform &a, const Transform &b) noexcept {
	Transform c = a;
	c -= b;
	return c;
}
