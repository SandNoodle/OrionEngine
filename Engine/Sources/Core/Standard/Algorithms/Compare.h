#pragma once

#include "OrionEngine.h"

namespace Orion::Engine::Algorithm
{
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 Less(const T& lhs, const T& rhs) noexcept
	{
		return lhs < rhs;
	}

	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 LessEqual(const T& lhs, const T& rhs) noexcept
	{
		return lhs <= rhs;
	}

	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 Greater(const T& lhs, const T& rhs) noexcept
	{
		return lhs > rhs;
	}

	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 GreaterEqual(const T& lhs, const T& rhs) noexcept
	{
		return lhs >= rhs;
	}

	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 Equal(const T& lhs, const T& rhs) noexcept
	{
		return lhs == rhs;
	}

	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 NotEqual(const T& lhs, const T& rhs) noexcept
	{
		return lhs != rhs;
	}
}  // namespace Orion::Engine::Algorithm
