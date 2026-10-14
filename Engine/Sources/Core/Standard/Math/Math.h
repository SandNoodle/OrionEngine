#pragma once

#include "OrionEngine.h"

#include "Platform/Compiler.h"

namespace Orion::Engine::Math
{
	// -- Basic functions.

	/// @brief Returns absolute value of a \p v.
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T Abs(T v) noexcept
	{
		return Platform::Compiler::Math::Abs<T>(v);
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
		return Platform::Compiler::Math::Sin<T>(v);
	}

	/// @brief Computes cosine (cos \p v).
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T Cos(T v) noexcept
	{
		return Platform::Compiler::Math::Cos<T>(v);
	}

	/// @brief Computes tangent (tan \p v).
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T Tan(T v) noexcept
	{
		return Platform::Compiler::Math::Tan<T>(v);
	}

	/// @brief Computes arc cosine (arccos \p v).
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T ACos(T v) noexcept
	{
		return Platform::Compiler::Math::ACos<T>(v);
	}

	/// @brief Computes arc sine (arcsin \p v).
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T ASin(T v) noexcept
	{
		return Platform::Compiler::Math::ASin<T>(v);
	}

	/// @brief Computes arc tangent (arctan \p v).
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T ATan(T v) noexcept
	{
		return Platform::Compiler::Math::ATan<T>(v);
	}

	/// @brief Computes arc tangent, using signs to determine quadrants (arctan2 \p v).
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T ATan2(T y, T x) noexcept
	{
		return Platform::Compiler::Math::ATan2<T>(y, x);
	}
}  // namespace Orion::Engine::Math
