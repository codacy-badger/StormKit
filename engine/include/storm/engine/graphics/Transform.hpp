#pragma once
#include <utility>

#include <storm/engine/render/Types.hpp>

#include <storm/core/Memory.hpp>
#include <storm/core/Platform.hpp>

//xyz position, pitch, yaw and roll
namespace storm::engine {
	class STORM_PUBLIC Transform {
		public:
			SUR_Object(Transform)

			explicit Transform() noexcept;
			~Transform()         noexcept;

			Transform(const Transform &)            noexcept;
			Transform &operator=(const Transform &) noexcept;

			Transform(Transform &&)                 noexcept;
			Transform &operator=(Transform &&)      noexcept;

			inline void setPosition(const vec3 &position) noexcept;
			inline void setPosition(vec3 &&position) noexcept;
			inline void setOrigin(const vec3 &origin) noexcept;
			inline void setOrigin(vec3 &&origin) noexcept;
			inline void move(const vec3 &position) noexcept;
			inline void move(vec3 &&position) noexcept;

			inline void setX(float x) noexcept;
			inline void setY(float y) noexcept;
			inline void setZ(float z) noexcept;

			inline void moveX(float x) noexcept;
			inline void moveY(float y) noexcept;
			inline void moveZ(float z) noexcept;

			inline void setScale(float x, float y, float z) noexcept;
			inline void setScale(const vec3 &scale) noexcept;
			inline void scale(float x, float y, float z) noexcept;
			inline void scale(const vec3 &scale) noexcept;
			inline void scaleX(float x) noexcept;
			inline void scaleY(float y) noexcept;
			inline void scaleZ(float z) noexcept;

			inline void setPitch(float pitch) noexcept;
			inline void setYaw(float yaw) noexcept;
			inline void setRoll(float roll) noexcept;

			inline const vec3 &position() const noexcept;
			inline const vec3 &origin() const noexcept;

			inline const vec3 &scale() const noexcept;

			inline float pitch() const noexcept;
			inline float yaw()   const noexcept;
			inline float roll()  const noexcept;

			inline mat4 matrix() const noexcept;
			inline operator mat4() const noexcept;

			void operator*=(const Transform &matrix) noexcept;
			void operator+=(const Transform &matrix) noexcept;
			void operator/=(const Transform &matrix) noexcept;
			void operator-=(const Transform &matrix) noexcept;

			inline void forceRecompute() const noexcept;
		private:
			void recompute() const noexcept;

			mutable bool      m_recompute;
			mutable mat4 m_transform;

			vec3 m_origin;
			vec3 m_position;
			vec3 m_scale;
			mutable quat m_quat;
			float     m_pitch;
			float     m_yaw;
			float     m_roll;
			
			friend struct std::hash<storm::engine::Transform>;
	};

	 Transform operator*(const Transform &a, const Transform &b) noexcept;
	 Transform operator+(const Transform &a, const Transform &b) noexcept;
	 Transform operator/(const Transform &a, const Transform &b) noexcept;
	 Transform operator-(const Transform &a, const Transform &b) noexcept;
}

#include "Transform.inl"
