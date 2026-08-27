#pragma once

#include "OrionEngine.h"

#include "Core/Standard/TypeTraits.h"
#include "Platform/Memory.h"

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

	/// @brief TODO
	/// @tparam T TODO
	/// @tparam SizeType TODO
	/// @param lhs TODO
	/// @param rhs TODO
	/// @param lhs_count TODO
	/// @param rhs_count TODO
	template <typename T, typename SizeType = USize>
	[[nodiscard]] constexpr Int8 Compare(const T* lhs, const T* rhs, SizeType lhs_count, SizeType rhs_count) noexcept
	{
		ORION_ASSERT_DEBUG_SLOW(lhs);
		ORION_ASSERT_DEBUG_SLOW(rhs);

		if (lhs_count == 0 && rhs_count == 0) [[unlikely]] {
			return 0;
		}

		SizeType count = ORION_MIN(lhs_count, rhs_count);
		if constexpr (IsTriviallyConstructible<T>) {
			SizeType size_in_bytes = sizeof(T) * count;
			Int8 result            = Platform::MemoryCompare(lhs, rhs, size_in_bytes);
			if (result == 0) {
				if (lhs_count < rhs_count) {
					return -1;
				}
				if (lhs_count > rhs_count) {
					return 1;
				}
			}
			return result;
		} else {
			for (; count; --count, ++lhs, ++rhs) {
				if (*lhs < *rhs) {
					return -1;
				}
				if (*lhs > *rhs) {
					return 1;
				}
			}
			if (lhs_count < rhs_count) {
				return -1;
			}
			if (lhs_count > rhs_count) {
				return 1;
			}
			return 0;
		}
	}
}  // namespace Orion::Engine::Algorithm
