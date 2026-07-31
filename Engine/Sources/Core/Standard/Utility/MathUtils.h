#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Concepts.h"

namespace Orion::Engine
{
	/// @brief Verifies that the value \p v is a power of two.
	template <typename T>
		requires(IsInteger<T>)
	[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 IsPowerOfTwo(T v) noexcept
	{
		return (v & (v - 1)) == 0;
	}

	/// @brief Rounds up given value \p v into the next power of two.
	template <typename T>
		requires(IsInteger<T>)
	[[nodiscard]] ORION_FORCE_INLINE constexpr T ToNextPowerOfTwo(T v) noexcept
	{
		if (v < 1) [[unlikely]] {
			return 2;
		}

		--v;
		v |= v >> 1UL;
		v |= v >> 2UL;
		v |= v >> 4UL;
		if constexpr (sizeof(T) >= sizeof(UInt16)) {
			v |= v >> 8UL;
		}
		if constexpr (sizeof(T) >= sizeof(UInt32)) {
			v |= v >> 16UL;
		}
		if constexpr (sizeof(T) >= sizeof(UInt64)) {
			v |= v >> 32UL;
		}
		++v;

		return v;
	}
}  // namespace Orion::Engine
