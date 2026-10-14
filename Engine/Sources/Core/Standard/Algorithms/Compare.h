#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Math/Math.h"
#include "Core/Standard/Traits/TypeProperties.h"
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

	/// @brief Compares two ranges of elements pointed to by \p lhs and \rhs.
	/// @tparam T Type of the element to compare.
	/// @param[IN, REQUIRED] lhs Pointer pointing at the start of first continuous sequence of elements.
	/// @param[IN, REQUIRED] rhs Pointer pointing at the start of second continuous sequence of elements.
	/// @param[IN, REQUIRED] lhs_count Count of elements pointed to by \p lhs.
	/// @param[IN, REQUIRED] rhs_count Count of elements pointed to by \p rhs.
	template <typename T, typename SizeType = USize>
	[[nodiscard]] constexpr Int32 Compare(const T* lhs, const T* rhs, SizeType lhs_count, SizeType rhs_count) noexcept
	{
		ORION_ASSERT_DEBUG_SLOW(lhs);
		ORION_ASSERT_DEBUG_SLOW(rhs);

		if (lhs_count == 0 && rhs_count == 0) [[unlikely]] {
			return 0;
		}

		SizeType count = Math::Min(lhs_count, rhs_count);
		if constexpr (IsBitwiseComparable<T>) {
			SizeType size_in_bytes = sizeof(T) * count;
			Int32 result           = Platform::MemoryCompare(lhs, rhs, size_in_bytes);
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
