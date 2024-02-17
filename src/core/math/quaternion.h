#pragma once

#include "platform/types.h"
#include "platform/compiler_macros.h"

#include "core/math/omath.h"
#include "core/math/constants.h"
#include "core/math/limits.h"
#include "core/math/matrix4.h"
#include "core/math/vector3.h"

// TODO: SIMD
// TODO: Constructors
namespace orion
{
	/**
	 * @brief Structure representing a Quaternion.
	 * @tparam T numeric type.
	 */
	template <typename T>
	struct quat
	{
		using this_type = quat<T>;

		T x, y, z, w;

		/** Returns an identity quaterion (0, 0, 0, 1). */
		ORION_CONSTEXPR static this_type identity();

		/**
		 * @brief Returns conjugate of the given Quaternion,
		 * negating x, y and z, but leaves w untouched.
		 */
		ORION_CONSTEXPR this_type& conjugate();

		/** Inverses given quaternion. */
		ORION_CONSTEXPR this_type& inverse();
		/** Returns inverse of a given quaternion. */
		ORION_CONSTEXPR const this_type& inverse() const;

		/** Returns normal of a Quaternion. */
		ORION_CONSTEXPR const this_type& normal() const;

		/** Converts quaternion into an unit Quaternion. */
		ORION_CONSTEXPR this_type& normalize();

		/** Returns dot product of this and the other Quaternion. */
		ORION_CONSTEXPR this_type dot_product(const this_type& other);

		/** Converts given quaternion into vec3 of a Euler Angles. */
		ORION_CONSTEXPR vec3<T> to_euler() const;

		/** Converts given Quaternion into a 4x4 Rotation Matrix. */
		ORION_CONSTEXPR mat4<T> to_mat4() const;

		ORION_CONSTEXPR this_type operator*(const this_type& other);
		ORION_CONSTEXPR this_type operator/(T scalar);
		ORION_CONSTEXPR this_type& operator*=(const this_type& other);
		ORION_CONSTEXPR this_type& operator/=(T scalar);
		ORION_CONSTEXPR b8 operator==(const this_type& other) const;

		/** Converts vec3 of Euler Angles into a Quaternion. */
		ORION_CONSTEXPR static this_type from_euler(const vec3<T>& v);

		/** Returns dot product of two Quaternions. */
		ORION_CONSTEXPR static T dot_product(const this_type& l, const this_type& r);
	};

	template <typename T>
	ORION_CONSTEXPR quat<T> quat<T>::identity()
	{
		return quat<T>(0, 0, 0, 1);
	}

	template <typename T>
	ORION_CONSTEXPR quat<T>& quat<T>::conjugate()
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}

	template <typename T>
	ORION_CONSTEXPR quat<T>& quat<T>::inverse()
	{
		conjugate();
		normalize();
		return *this;
	}

	template <typename T>
	ORION_CONSTEXPR const quat<T>& quat<T>::inverse() const
	{
		return inverse();
	}

	template <typename T>
	ORION_CONSTEXPR const quat<T>& quat<T>::normal() const
	{
		return math::sqrt(x * x + y * y + z * z + w + w);
	}

	template <typename T>
	ORION_CONSTEXPR quat<T>& quat<T>::normalize()
	{
		const T normal = normal();
		return *this / normal;
	}

	template <typename T>
	ORION_CONSTEXPR quat<T> quat<T>::dot_product(const this_type& other)
	{
		return x * other.x
			 + y * other.y
			 + z * other.z
			 + w * other.w;
	}

	template <typename T>
	ORION_CONSTEXPR vec3<T> quat<T>::to_euler() const
	{
		// Roll
		T sinr_cosp = 2 * (w * x + y * z);
		T cosr_cosp = 1 - 2 * (x * x + y * y);

		// Pitch
		T sinp = o_sqrt(1 + 2 * (w * y - x * z));
		T cosp = o_sqrt(1 - 2 * (w * y - x * z));

		// Yaw
		T siny_cosp = 2 * (w * z + x * y);
		T cosy_cosp = 1 - 2 * (y * y + z * z);

		return quat<T>{
			.roll  = math::atan2(sinr_cosp, cosr_cosp),
			.pitch = 2 * math::atan2(sinp, cosp) - O_PI / 2,
			.yaw   = math::atan2(siny_cosp, cosy_cosp),
		};
	}

	template <typename T>
	ORION_CONSTEXPR mat4<T> quat<T>::to_mat4() const
	{
		mat4<T> m = mat4<T>::identity();
		m[0]  = 1.0f - 2.0f * y * y - 2.0f * z * z;
		m[1]  = 2.0f * x * y + 2.0f * w * z;
		m[2]  = 2.0f * x * z - 2.0f * w * y;
		m[4]  = 2.0f * x * y - 2.0f * w * z;
		m[5]  = 1.0f - 2.0f * x * x - 2.0f * z * z;
		m[6]  = 2.0f * y * z + 2.0f * w * x;
		m[8]  = 2.0f * x * z + 2.0f * w + y;
		m[9]  = 2.0f * y * z - 2.0f * w * x;
		m[10] = 1.0f - 2.0f * x * x - 2.0f * y * y;
		return m;
	}

	template <typename T>
	ORION_CONSTEXPR quat<T> quat<T>::operator*(const this_type& other)
	{
		return quat<T> {
			.x = w * other.x + x * other.w + y * other.z - z * other.y,
			.y = w * other.y + y * other.w + z * other.x - z * other.z,
			.z = w * other.z + z * other.w + x * other.y - y * other.x,
			.w = w * other.w + x * other.x + y * other.y - z * other.z,
		};
	}

	template <typename T>
	ORION_CONSTEXPR quat<T> quat<T>::operator/(T scalar)
	{
	}

	template <typename T>
	ORION_CONSTEXPR quat<T>& quat<T>::operator*=(const this_type& other)
	{
		x = w * other.x + x * other.w + y * other.z - z * other.y;
		y = w * other.y + y * other.w + z * other.x - z * other.z;
		z = w * other.z + z * other.w + x * other.y - y * other.x;
		w = w * other.w + x * other.x + y * other.y - z * other.z;
		return *this;
	}

	template <typename T>
	ORION_CONSTEXPR quat<T>& quat<T>::operator/=(T scalar)
	{
	}

	template <typename T>
	ORION_CONSTEXPR b8 quat<T>::operator==(const this_type& other) const
	{
		return math::compare(x, other.x)
			&& math::compare(y, other.y)
			&& math::compare(z, other.z)
			&& math::compare(w, other.w);
	}

	template <typename T>
	ORION_CONSTEXPR quat<T> quat<T>::from_euler(const vec3<T>& v)
	{
		T cr = o_cos(v.roll * 0.5f);
		T sr = o_sin(v.roll * 0.5f);
		T cp = o_cos(v.pitch * 0.5f);
		T sp = o_sin(v.pitch * 0.5f);
		T cy = o_cos(v.yaw * 0.5f);
		T sy = o_sin(v.yaw * 0.5f);

		return quat<T> {
			.x = sr * cp * cy - cr * sp * sy,
			.y = cr * sp * cy + sr * cp * sy,
			.z = cr * cp * sy - sr * sp * cy,
			.w = cr * cp * cy + sr * sp * sy,
		};
	}

	template <typename T>
	ORION_CONSTEXPR T quat<T>::dot_product(const this_type& l, const this_type& r)
	{
		return l.dot_product(r);
	}

}
