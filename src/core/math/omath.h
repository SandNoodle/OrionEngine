#pragma once

#include "platform/types.h"

#include <cmath>
#include <limits>
#include <type_traits>

namespace orion::math
{
	/** Returns true if the given value is a power of two, false otherwise. */
	template <typename T>
	constexpr b8 is_power_of_two(T x)
	{
		return (x > 0) && (x & (x - 1)) == 0;
	}

	template <typename T>
	constexpr T next_power_of_two(T x)
	{
		static_assert(std::is_integral_v<T>, "next power of two can be used only on a integral types.");
		x--;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		x |= x >> 8;
		x |= x >> 16;
		if constexpr (sizeof(T) == sizeof(u64))
		{
			x |= x >> 32;
		}
		return ++x;
	}

	/** Compares two values and returns true if they are equal, false otherwise. */
	template <typename T>
	constexpr b8 compare(T x, T y)
	{
		if constexpr (std::is_same<T, f32>::value || std::is_same<T, f64>::value)
		{
			return std::abs(x - y) < std::numeric_limits<T>::epsilon;
		}
		return x == y;
	}
}
