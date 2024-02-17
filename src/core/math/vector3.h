#pragma once

#include "platform/types.h"
#include "platform/compiler_macros.h"

#include "core/math/omath.h"
#include "core/math/limits.h"

// TODO: SIMD
// TODO: Constructors
namespace orion
{
	/**
	 * @brief Structure representing a 3D Vector.
	 * @tparam T numeric type.
	 */
	template <typename T>
	struct vec3
	{
		using this_type = vec3<T>;

		union {
			T elements[3];
			struct { T x, y, z; };
			struct { T r, g, b; };
			struct { T width, height, depth; };
			struct { T roll, pitch, yaw; };
		};

		ORION_CONSTEXPR T length() const;
		ORION_CONSTEXPR T length_squared() const;

		ORION_CONSTEXPR T distance(const vec3& other) const;
		ORION_CONSTEXPR T distance_squared(const vec3& other) const;

		/** Converts vector into an unit vector. */
		ORION_CONSTEXPR this_type& normalize();

		ORION_CONSTEXPR T& operator[](usize index);
		ORION_CONSTEXPR const T& operator[](usize index) const;
		ORION_CONSTEXPR this_type operator+(const this_type& other);
		ORION_CONSTEXPR this_type operator-(const this_type& other);
		ORION_CONSTEXPR this_type operator*(const this_type& other);
		ORION_CONSTEXPR this_type operator/(const this_type& other);
		ORION_CONSTEXPR this_type& operator+=(const this_type& other);
		ORION_CONSTEXPR this_type& operator-=(const this_type& other);
		ORION_CONSTEXPR this_type& operator*=(const this_type& other);
		ORION_CONSTEXPR this_type& operator/=(const this_type& other);
		ORION_CONSTEXPR b8 operator==(const this_type& other) const;

		ORION_CONSTEXPR static T distance(const this_type& l, const this_type& r);
		ORION_CONSTEXPR static T distance_squared(const this_type& l, const this_type& r);

		ORION_CONSTEXPR static this_type normalize(const this_type& other);
	};

	template <typename T>
	ORION_CONSTEXPR T vec3<T>::length() const
	{
		return math::sqrt(length_squared());
	}

	template <typename T>
	ORION_CONSTEXPR T vec3<T>::length_squared() const
	{
		return x * x + y * y + z * z;
	}

	template <typename T>
	ORION_CONSTEXPR T vec3<T>::distance(const this_type& other) const
	{
		return (*this - other).length();
	}

	template <typename T>
	ORION_CONSTEXPR T vec3<T>::distance_squared(const this_type& other) const
	{
		return (*this - other).length_squared();
	}

	template <typename T>
	ORION_CONSTEXPR vec3<T>& vec3<T>::normalize()
	{
		const T len = length();
		x / len;
		y / len;
		z / len;
		return *this;
	}

	template <typename T>
	ORION_CONSTEXPR T& vec3<T>::operator[](usize index)
	{
		return elements[index];
	}

	template <typename T>
	ORION_CONSTEXPR const T& vec3<T>::operator[](usize index) const
	{
		return elements[index];
	}

	template <typename T>
	ORION_CONSTEXPR vec3<T> vec3<T>::operator+(const this_type& other)
	{
		return vec3<T>(x + other.x,
			           y + other.y,
		               z + other.z);
	}

	template <typename T>
	ORION_CONSTEXPR vec3<T> vec3<T>::operator-(const this_type& other)
	{
		return vec3<T>(x - other.x,
			           y - other.y,
		               z - other.z);
	}

	template <typename T>
	ORION_CONSTEXPR vec3<T> vec3<T>::operator*(const this_type& other)
	{
		return vec3<T>(x * other.x,
			           y * other.y,
		               z * other.z);
	}

	template <typename T>
	ORION_CONSTEXPR vec3<T> vec3<T>::operator/(const this_type& other)
	{
		return vec3<T>(x / other.x,
			           y / other.y,
		               z / other.z);
	}

	template <typename T>
	ORION_CONSTEXPR vec3<T>& vec3<T>::operator+=(const this_type& other)
	{
		x + other.x;
		y + other.y;
		z + other.z;
		return *this;
	}

	template <typename T>
	ORION_CONSTEXPR vec3<T>& vec3<T>::operator-=(const this_type& other)
	{
		x - other.x;
		y - other.y;
		z - other.z;
		return *this;
	}

	template <typename T>
	ORION_CONSTEXPR vec3<T>& vec3<T>::operator*=(const this_type& other)
	{
		x * other.x;
		y * other.y;
		z * other.z;
		return *this;
	}

	template <typename T>
	ORION_CONSTEXPR vec3<T>& vec3<T>::operator/=(const this_type& other)
	{
		x / other.x;
		y / other.y;
		z / other.z;
		return *this;
	}

	template <typename T>
	ORION_CONSTEXPR b8 vec3<T>::operator==(const vec3<T>& other) const
	{
		return math::compare(x, other.x)
			&& math::compare(y, other.y)
			&& math::compare(z, other.z);
	}

	template <typename T>
	ORION_CONSTEXPR T vec3<T>::distance(const this_type& l, const this_type& r)
	{
		return l.distance(r);
	}

	template <typename T>
	ORION_CONSTEXPR T vec3<T>::distance_squared(const this_type& l, const this_type& r)
	{
		return l.distance_squared(r);
	}

	template <typename T>
	ORION_CONSTEXPR vec3<T> vec3<T>::normalize(const this_type& other)
	{
		return other.normalize();
	}
}  // namespace orion

