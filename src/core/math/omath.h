#pragma once

#include "platform/types.h"
#include "platform/compiler_macros.h"
#include "core/math/limits.h"

#include <math.h>

namespace orion::math
{
	/** Returns minimum of the two values. */
	template <typename T>
	ORION_CONSTEXPR T min(T x, T y)
	{
		return x < y ? x : y;
	}

	/** Returns maximum of the two values. */
	template <typename T>
	ORION_CONSTEXPR T max(T x, T y)
	{
		return y < x ? x : y;
	}

	/** Returns sinus of a value. */
	template <typename T>
	ORION_CONSTEXPR T sin(T x)
	{
		return sin(x);
	}

	/** Returns cosinus of a value. */
	template <typename T>
	ORION_CONSTEXPR T cos(T x)
	{
		return cos(x);
	}

	/** Returns tangens of a value. */
	template <typename T>
	ORION_CONSTEXPR T tan(T x)
	{
		return tan(x);
	}

	/** Returns arctangent of two values. */
	template <typename T>
	ORION_CONSTEXPR T atan2(T y, T x)
	{
		return atan2(y, x);
	}

	/** Returns square root of two values. */
	template <typename T>
	ORION_CONSTEXPR T sqrt(T x)
	{
		return sqrt(x);
	}

	/** Returns an absolute value. */
	template <typename T>
	ORION_CONSTEXPR T abs(T x)
	{
		return abs(x);
	}

	/** Returns floor of a value. */
	template <typename T>
	ORION_CONSTEXPR T floor(T x)
	{
		return floor(x);
	}

	/** Returns binary logarithm of a value. */
	template <typename T>
	ORION_CONSTEXPR T log2(T x)
	{
		return log2(x);
	}

	/** Returns true if the given value is a power of two, false otherwise. */
	template <typename T>
	ORION_CONSTEXPR b8 is_power_of_two(T x)
	{
		return (x > 0) && (x & (x - 1)) == 0;
	}

	/** Compares two values and returns true if they are equal, false otherwise. */
	template <typename T>
	ORION_CONSTEXPR b8 compare(T x, T y)
	{
		return x == y;
	}

	// 
	// Specializations
	// 

	template <> ORION_CONSTEXPR b8 is_power_of_two(f32 x) { return false; }
	template <> ORION_CONSTEXPR b8 is_power_of_two(f64 x) { return false; }

	template <> ORION_CONSTEXPR b8 compare(f32 x, f32 y)
	{
		return abs<f32>(x - y) < F32_EPSILON;
	}

	template <> ORION_CONSTEXPR b8 compare(f64 x, f64 y)
	{
		return abs<f64>(x - y) < F64_EPSILON;
	}
}
