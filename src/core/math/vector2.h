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
	 * @brief Structure representing a 2D Vector.
	 * @tparam T numeric type.
	 */
	template <typename T>
	struct vec2
	{
		using this_type = vec2<T>;

		union {
			T elements[2];
			struct { T x, y; };
			struct { T r, g; };
			struct { T width, height; };
			struct { T roll, pitch; };
		};

		ORION_CONSTEXPR T length() const;
		ORION_CONSTEXPR T length_squared() const;

		ORION_CONSTEXPR T distance(const vec2& other) const;
		ORION_CONSTEXPR T distance_squared(const vec2& other) const;

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
	ORION_CONSTEXPR T vec2<T>::length() const
	{
		return math::sqrt(length_squared());
	}

	template <typename T>
	ORION_CONSTEXPR T vec2<T>::length_squared() const
	{
		return x * x + y * y;
	}

	template <typename T>
	ORION_CONSTEXPR T vec2<T>::distance(const this_type& other) const
	{
		return (*this - other).length();
	}

	template <typename T>
	ORION_CONSTEXPR T vec2<T>::distance_squared(const this_type& other) const
	{
		return (*this - other).length_squared();
	}

	template <typename T>
	ORION_CONSTEXPR vec2<T>& vec2<T>::normalize()
	{
		const T len = length();
		x / len;
		y / len;
		return *this;
	}

	template <typename T>
	ORION_CONSTEXPR T& vec2<T>::operator[](usize index)
	{
		return elements[index];
	}

	template <typename T>
	ORION_CONSTEXPR const T& vec2<T>::operator[](usize index) const
	{
		return elements[index];
	}

	template <typename T>
	ORION_CONSTEXPR vec2<T> vec2<T>::operator+(const this_type& other)
	{
		return vec2<T>(x + other.x,
			           y / other.y);
	}

	template <typename T>
	ORION_CONSTEXPR vec2<T> vec2<T>::operator-(const this_type& other)
	{
		return vec2<T>(x - other.x,
			           y / other.y);
	}

	template <typename T>
	ORION_CONSTEXPR vec2<T> vec2<T>::operator*(const this_type& other)
	{
		return vec2<T>(x * other.x,
			           y / other.y);
	}

	template <typename T>
	ORION_CONSTEXPR vec2<T> vec2<T>::operator/(const this_type& other)
	{
		return vec2<T>(x / other.x,
			           y / other.y);
	}

	template <typename T>
	ORION_CONSTEXPR vec2<T>& vec2<T>::operator+=(const this_type& other)
	{
		x + other.x;
		y + other.y;
		return *this;
	}

	template <typename T>
	ORION_CONSTEXPR vec2<T>& vec2<T>::operator-=(const this_type& other)
	{
		x - other.x;
		y - other.y;
		return *this;
	}

	template <typename T>
	ORION_CONSTEXPR vec2<T>& vec2<T>::operator*=(const this_type& other)
	{
		x * other.x;
		y * other.y;
		return *this;
	}

	template <typename T>
	ORION_CONSTEXPR vec2<T>& vec2<T>::operator/=(const this_type& other)
	{
		x / other.x;
		y / other.y;
		return *this;
	}

	template <typename T>
	ORION_CONSTEXPR b8 vec2<T>::operator==(const vec2<T>& other) const
	{
		return math::compare(x, other.x)
			&& math::compare(y, other.y);
	}

	template <typename T>
	ORION_CONSTEXPR T vec2<T>::distance(const this_type& l, const this_type& r)
	{
		return l.distance(r);
	}

	template <typename T>
	ORION_CONSTEXPR T vec2<T>::distance_squared(const this_type& l, const this_type& r)
	{
		return l.distance_squared(r);
	}

	template <typename T>
	ORION_CONSTEXPR vec2<T> vec2<T>::normalize(const this_type& other)
	{
		return other.normalize();
	}
}
