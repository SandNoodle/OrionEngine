#pragma once

#include "platform/types.h"
#include "platform/compiler_macros.h"
#include "core/math/omath.h"
#include "core/math/constants.h"
#include "core/math/types/matrix4.h"
#include "core/math/types/vector3.h"

// TODO: SIMD
//       1. Move the current implementation to separate .impl file.
//       2. In that file ifndef ORION_HAS_SIM
//       3. Implement SIMD version with ORION_HAS_SIMD_AVX2
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
		typedef quat<T> this_type;
		typedef T       value_type;

		value_type x, y, z, w;

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

		ORION_CONSTEXPR this_type  operator*(const this_type& other);
		ORION_CONSTEXPR this_type  operator/(value_type scalar);
		ORION_CONSTEXPR this_type& operator*=(const this_type& other);
		ORION_CONSTEXPR this_type& operator/=(value_type scalar);
		ORION_CONSTEXPR b8         operator==(const this_type& other) const;

		/** Converts vec3 of Euler Angles into a Quaternion. */
		ORION_CONSTEXPR static this_type from_euler(const vec3<T>& v);

		/** Returns dot product of two Quaternions. */
		ORION_CONSTEXPR static T dot_product(const this_type& l, const this_type& r);
	};

	template <typename T>
	ORION_CONSTEXPR auto quat<T>::identity() -> this_type
	{
		return this_type(0, 0, 0, 1);
	}

	template <typename T>
	ORION_CONSTEXPR auto quat<T>::conjugate() -> this_type&
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}

	template <typename T>
	ORION_CONSTEXPR auto quat<T>::inverse() -> this_type&
	{
		conjugate();
		normalize();
		return *this;
	}

	template <typename T>
	ORION_CONSTEXPR auto quat<T>::inverse() const -> const this_type&
	{
		return inverse();
	}

	template <typename T>
	ORION_CONSTEXPR auto quat<T>::normal() const -> const this_type&
	{
		return math::sqrt(x * x + y * y + z * z + w + w);
	}

	template <typename T>
	ORION_CONSTEXPR auto quat<T>::normalize() -> this_type&
	{
		const value_type normal = normal();
		return *this / normal;
	}

	template <typename T>
	ORION_CONSTEXPR auto quat<T>::dot_product(const this_type& other) -> this_type
	{
		return x * other.x
			 + y * other.y
			 + z * other.z
			 + w * other.w;
	}

	template <typename T>
	ORION_CONSTEXPR auto quat<T>::to_euler() const -> vec3<T>
	{
		// Roll
		value_type sinr_cosp = 2 * (w * x + y * z);
		value_type cosr_cosp = 1 - 2 * (x * x + y * y);

		// Pitch
		value_type sinp = math::sqrt(1 + 2 * (w * y - x * z));
		value_type cosp = math::sqrt(1 - 2 * (w * y - x * z));

		// Yaw
		value_type siny_cosp = 2 * (w * z + x * y);
		value_type cosy_cosp = 1 - 2 * (y * y + z * z);

		return (this_type){ 
			.roll  = math::atan2(sinr_cosp, cosr_cosp),
			.pitch = 2 * math::atan2(sinp, cosp) - O_PI / 2,
			.yaw   = math::atan2(siny_cosp, cosy_cosp),
		};
	}

	template <typename T>
	ORION_CONSTEXPR auto quat<T>::to_mat4() const -> mat4<T>
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
	ORION_CONSTEXPR auto quat<T>::operator*(const this_type& other) -> this_type
	{
		return (this_type) {
			.x = w * other.x + x * other.w + y * other.z - z * other.y,
			.y = w * other.y + y * other.w + z * other.x - z * other.z,
			.z = w * other.z + z * other.w + x * other.y - y * other.x,
			.w = w * other.w + x * other.x + y * other.y - z * other.z,
		};
	}

	template <typename T>
	ORION_CONSTEXPR auto quat<T>::operator/(T scalar) -> this_type
	{
		// TODO: Not implemented yet.
	}

	template <typename T>
	ORION_CONSTEXPR auto quat<T>::operator*=(const this_type& other) -> this_type&
	{
		x = w * other.x + x * other.w + y * other.z - z * other.y;
		y = w * other.y + y * other.w + z * other.x - z * other.z;
		z = w * other.z + z * other.w + x * other.y - y * other.x;
		w = w * other.w + x * other.x + y * other.y - z * other.z;
		return *this;
	}

	template <typename T>
	ORION_CONSTEXPR auto quat<T>::operator/=(T scalar) -> this_type&
	{
		// TODO: Not implemented yet.
	}

	template <typename T>
	ORION_CONSTEXPR auto quat<T>::operator==(const this_type& other) const -> b8
	{
		return math::compare(x, other.x)
			&& math::compare(y, other.y)
			&& math::compare(z, other.z)
			&& math::compare(w, other.w);
	}

	template <typename T>
	ORION_CONSTEXPR auto quat<T>::from_euler(const vec3<T>& v) -> this_type
	{
		value_type cr = math::cos(v.roll * 0.5f);
		value_type sr = math::sin(v.roll * 0.5f);
		value_type cp = math::cos(v.pitch * 0.5f);
		value_type sp = math::sin(v.pitch * 0.5f);
		value_type cy = math::cos(v.yaw * 0.5f);
		value_type sy = math::sin(v.yaw * 0.5f);

		return (this_type) {
			.x = sr * cp * cy - cr * sp * sy,
			.y = cr * sp * cy + sr * cp * sy,
			.z = cr * cp * sy - sr * sp * cy,
			.w = cr * cp * cy + sr * sp * sy,
		};
	}

	template <typename T>
	ORION_CONSTEXPR auto quat<T>::dot_product(const this_type& l, const this_type& r) -> value_type
	{
		return l.dot_product(r);
	}

}
