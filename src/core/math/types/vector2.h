#pragma once

#include "platform/types.h"
#include "platform/compiler_macros.h"

#include "core/math/omath.h"

// TODO: SIMD
//       1. Move the current implementation to separate .impl file.
//       2. In that file ifndef ORION_HAS_SIM
//       3. Implement SIMD version with ORION_HAS_SIMD_AVX2
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
		typedef vec2<T>  this_type;
		typedef T        value_type;
		typedef T&       reference;
		typedef const T& const_reference;
		typedef usize    size_type;

		union {
			value_type elements[2];
			struct { value_type x, y; };
			struct { value_type r, g; };
			struct { value_type width, height; };
			struct { value_type roll, pitch; };
		};

		ORION_CONSTEXPR value_type length() const;
		ORION_CONSTEXPR value_type length_squared() const;

		ORION_CONSTEXPR value_type distance(const vec2& other) const;
		ORION_CONSTEXPR value_type distance_squared(const vec2& other) const;

		/** Converts vector into an unit vector. */
		ORION_CONSTEXPR this_type& normalize();

		ORION_CONSTEXPR reference       operator[](size_type index);
		ORION_CONSTEXPR const_reference operator[](size_type index) const;
		ORION_CONSTEXPR this_type       operator+(const this_type& other);
		ORION_CONSTEXPR this_type       operator-(const this_type& other);
		ORION_CONSTEXPR this_type       operator*(const this_type& other);
		ORION_CONSTEXPR this_type       operator/(const this_type& other);
		ORION_CONSTEXPR this_type&      operator+=(const this_type& other);
		ORION_CONSTEXPR this_type&      operator-=(const this_type& other);
		ORION_CONSTEXPR this_type&      operator*=(const this_type& other);
		ORION_CONSTEXPR this_type&      operator/=(const this_type& other);
		ORION_CONSTEXPR b8              operator==(const this_type& other) const;

		ORION_CONSTEXPR static value_type distance(const this_type& l, const this_type& r);
		ORION_CONSTEXPR static value_type distance_squared(const this_type& l, const this_type& r);

		ORION_CONSTEXPR static this_type normalize(const this_type& other);
	};

	template <typename T>
	ORION_CONSTEXPR auto vec2<T>::length() const -> value_type
	{
		return math::sqrt(length_squared());
	}

	template <typename T>
	ORION_CONSTEXPR auto vec2<T>::length_squared() const -> value_type
	{
		return x * x + y * y;
	}

	template <typename T>
	ORION_CONSTEXPR auto vec2<T>::distance(const this_type& other) const -> value_type
	{
		return (*this - other).length();
	}

	template <typename T>
	ORION_CONSTEXPR auto vec2<T>::distance_squared(const this_type& other) const -> value_type
	{
		return (*this - other).length_squared();
	}

	template <typename T>
	ORION_CONSTEXPR auto vec2<T>::normalize() -> this_type&
	{
		const T len = length();
		x / len;
		y / len;
		return *this;
	}

	template <typename T>
	ORION_CONSTEXPR auto vec2<T>::operator[](size_type index) -> reference
	{
		return elements[index];
	}

	template <typename T>
	ORION_CONSTEXPR auto vec2<T>::operator[](size_type index) const -> const_reference
	{
		return elements[index];
	}

	template <typename T>
	ORION_CONSTEXPR auto vec2<T>::operator+(const this_type& other) -> this_type
	{
		return vec2<T>(x + other.x,
			           y / other.y);
	}

	template <typename T>
	ORION_CONSTEXPR auto vec2<T>::operator-(const this_type& other) -> this_type
	{
		return vec2<T>(x - other.x,
			           y / other.y);
	}

	template <typename T>
	ORION_CONSTEXPR auto vec2<T>::operator*(const this_type& other) -> this_type
	{
		return vec2<T>(x * other.x,
			           y / other.y);
	}

	template <typename T>
	ORION_CONSTEXPR auto vec2<T>::operator/(const this_type& other) -> this_type
	{
		return vec2<T>(x / other.x,
			           y / other.y);
	}

	template <typename T>
	ORION_CONSTEXPR auto vec2<T>::operator+=(const this_type& other) -> this_type&
	{
		x + other.x;
		y + other.y;
		return *this;
	}

	template <typename T>
	ORION_CONSTEXPR auto vec2<T>::operator-=(const this_type& other) -> this_type&
	{
		x - other.x;
		y - other.y;
		return *this;
	}

	template <typename T>
	ORION_CONSTEXPR auto vec2<T>::operator*=(const this_type& other) -> this_type&
	{
		x * other.x;
		y * other.y;
		return *this;
	}

	template <typename T>
	ORION_CONSTEXPR auto vec2<T>::operator/=(const this_type& other) -> this_type&
	{
		x / other.x;
		y / other.y;
		return *this;
	}

	template <typename T>
	ORION_CONSTEXPR auto vec2<T>::operator==(const vec2<T>& other) const -> b8
	{
		return math::compare(x, other.x)
			&& math::compare(y, other.y);
	}

	template <typename T>
	ORION_CONSTEXPR auto vec2<T>::distance(const this_type& l, const this_type& r) -> value_type
	{
		return l.distance(r);
	}

	template <typename T>
	ORION_CONSTEXPR auto vec2<T>::distance_squared(const this_type& l, const this_type& r) -> value_type
	{
		return l.distance_squared(r);
	}

	template <typename T>
	ORION_CONSTEXPR auto vec2<T>::normalize(const this_type& other) -> this_type
	{
		return other.normalize();
	}
}
