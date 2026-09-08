#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"
#include "Core/Standard/Concepts.h"

namespace Orion::Engine::Math
{
	// -- Basic functions.

	/// @brief Returns absolute value of a \p v.
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T Abs(T v) noexcept
	{
		ORION_IGNORE_PARAM(v);
		ORION_NOT_IMPLEMENTED();
	}

	/// @brief Returns lower of two values \p x and \p y.
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T Min(T x, T y) noexcept
	{
		return x < y ? x : y;
	}

	/// @brief Returns greater of two values \p x and \p y.
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T Max(T x, T y) noexcept
	{
		return x > y ? x : y;
	}

	/// @brief Clamps value \p v between \p min and \p max.
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T Clamp(T v, T min, T max) noexcept
	{
		return v <= min ? min : v >= max ? max : v;
	}

	// -- Trigonometric functions.

	/// @brief Computes sine (sin \p v).
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T Sin(T v) noexcept
	{
		ORION_IGNORE_PARAM(v);
		ORION_NOT_IMPLEMENTED();
	}

	/// @brief Computes cosine (cos \p v).
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T Cos(T v) noexcept
	{
		ORION_IGNORE_PARAM(v);
		ORION_NOT_IMPLEMENTED();
	}

	/// @brief Computes tangent (tan \p v).
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T Tan(T v) noexcept
	{
		ORION_IGNORE_PARAM(v);
		ORION_NOT_IMPLEMENTED();
	}

	/// @brief Computes arc cosine (arccos \p v).
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T ACos(T v) noexcept
	{
		ORION_IGNORE_PARAM(v);
		ORION_NOT_IMPLEMENTED();
	}

	/// @brief Computes arc sine (arcsin \p v).
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T ASin(T v) noexcept
	{
		ORION_IGNORE_PARAM(v);
		ORION_NOT_IMPLEMENTED();
	}

	/// @brief Computes arc tangent (arctan \p v).
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T ATan(T v) noexcept
	{
		ORION_IGNORE_PARAM(v);
		ORION_NOT_IMPLEMENTED();
	}

	/// @brief Computes arc tangent, using signs to determine quadrants (arctan2 \p v).
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T ATan2(T y, T x) noexcept
	{
		ORION_IGNORE_PARAM(y);
		ORION_IGNORE_PARAM(x);
		ORION_NOT_IMPLEMENTED();
	}

	// -- Implementation.
	template <>
	[[nodiscard]] ORION_FORCE_INLINE constexpr Float32 Abs<Float32>(Float32 v) noexcept
	{
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
		return __builtin_fabsf(v);
#else
#error "Abs<Float32> is not defined for this compiler."
#endif
	}

	template <>
	[[nodiscard]] ORION_FORCE_INLINE constexpr Float64 Abs<Float64>(Float64 v) noexcept
	{
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
		return __builtin_fabs(v);
#else
#error "Abs<Float64> is not defined for this compiler."
#endif
	}

	template <>
	[[nodiscard]] ORION_FORCE_INLINE constexpr Float32 Sin<Float32>(Float32 v) noexcept
	{
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
		return __builtin_sinf(v);
#else
#error "Sin<Float32> is not defined for this compiler."
#endif
	}

	template <>
	[[nodiscard]] ORION_FORCE_INLINE constexpr Float64 Sin<Float64>(Float64 v) noexcept
	{
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
		return __builtin_sin(v);
#else
#error "Sin<Float64> is not defined for this compiler."
#endif
	}

	template <>
	[[nodiscard]] ORION_FORCE_INLINE constexpr Float32 Cos<Float32>(Float32 v) noexcept
	{
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
		return __builtin_cosf(v);
#else
#error "Cos<Float32> is not defined for this compiler."
#endif
	}

	template <>
	[[nodiscard]] ORION_FORCE_INLINE constexpr Float64 Cos<Float64>(Float64 v) noexcept
	{
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
		return __builtin_cos(v);
#else
#error "Cos<Float64> is not defined for this compiler."
#endif
	}

	template <>
	[[nodiscard]] ORION_FORCE_INLINE constexpr Float32 Tan<Float32>(Float32 v) noexcept
	{
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
		return __builtin_tanf(v);
#else
#error "Tan<Float32> is not defined for this compiler."
#endif
	}

	template <>
	[[nodiscard]] ORION_FORCE_INLINE constexpr Float64 Tan<Float64>(Float64 v) noexcept
	{
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
		return __builtin_tan(v);
#else
#error "Tan<Float64> is not defined for this compiler."
#endif
	}

	template <>
	[[nodiscard]] ORION_FORCE_INLINE constexpr Float32 ACos<Float32>(Float32 v) noexcept
	{
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
		return __builtin_acosf(v);
#else
#error "ACos<Float32> is not defined for this compiler."
#endif
	}

	template <>
	[[nodiscard]] ORION_FORCE_INLINE constexpr Float64 ACos<Float64>(Float64 v) noexcept
	{
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
		return __builtin_acos(v);
#else
#error "ACos<Float64> is not defined for this compiler."
#endif
	}

	template <>
	[[nodiscard]] ORION_FORCE_INLINE constexpr Float32 ASin<Float32>(Float32 v) noexcept
	{
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
		return __builtin_asinf(v);
#else
#error "ASin<Float32> is not defined for this compiler."
#endif
	}

	template <>
	[[nodiscard]] ORION_FORCE_INLINE constexpr Float64 ASin<Float64>(Float64 v) noexcept
	{
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
		return __builtin_asin(v);
#else
#error "ASin<Float64> is not defined for this compiler."
#endif
	}

	template <>
	[[nodiscard]] ORION_FORCE_INLINE constexpr Float32 ATan<Float32>(Float32 v) noexcept
	{
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
		return __builtin_atanf(v);
#else
#error "ATan<Float32> is not defined for this compiler."
#endif
	}

	template <>
	[[nodiscard]] ORION_FORCE_INLINE constexpr Float64 ATan<Float64>(Float64 v) noexcept
	{
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
		return __builtin_atan(v);
#else
#error "ATan<Float64> is not defined for this compiler."
#endif
	}

	template <>
	[[nodiscard]] ORION_FORCE_INLINE constexpr Float32 ATan2<Float32>(Float32 y, Float32 x) noexcept
	{
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
		return __builtin_atan2f(y, x);
#else
#error "ATan2<Float32> is not defined for this compiler."
#endif
	}

	template <>
	[[nodiscard]] ORION_FORCE_INLINE constexpr Float64 ATan2<Float64>(Float64 y, Float64 x) noexcept
	{
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
		return __builtin_atan2(y, x);
#else
#error "ATan2<Float64> is not defined for this compiler."
#endif
	}
}  // namespace Orion::Engine::Math
