#pragma once

#include "Core/Standard/Utility/MoveAndForward.h"
#include "Core/Assert.h"
#include "Core/Standard/Util.h"

namespace Orion::Engine::Algorithm
{
	namespace Internal
	{
		template <typename T, typename CompareFn>
		constexpr USize QuicksortPartition(T* values, USize lhs, USize rhs, CompareFn&& compare) noexcept
		{
			T& pivot          = values[rhs];
			USize pivot_index = lhs;
			for (USize index = lhs; index < rhs; ++index) {
				if (compare(values[index], pivot)) {
					Swap(values[pivot_index], values[index]);
					pivot_index++;
				}
			}
			Swap(values[pivot_index], values[rhs]);
			return pivot_index;
		}

		template <typename T, typename CompareFn>
		constexpr void Quicksort(T* values, USize lhs, USize rhs, CompareFn&& compare) noexcept
		{
			if (lhs >= rhs) {
				return;
			}
			USize pivot_index = QuicksortPartition(values, lhs, rhs, Forward<CompareFn>(compare));
			if (pivot_index > 0) {
				Quicksort(values, lhs, pivot_index - 1, Forward<CompareFn>(compare));
			}
			Quicksort(values, pivot_index + 1, rhs, Forward<CompareFn>(compare));
		}
	}  // namespace Internal

	namespace Compare
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
	}  // namespace Compare

	template <typename T, typename CompareFn>
	constexpr void Quicksort(T* values, USize values_size, CompareFn&& compare = Compare::Less<T>)
	{
		ORION_ASSERT_DEBUG(values);
		Internal::Quicksort(values, 0, values_size - 1, Forward<CompareFn>(compare));
	}

	template <typename T, typename CompareFn>
	constexpr void Sort(T* values, USize values_size, CompareFn&& compare = Compare::Less<T>)
	{
		ORION_ASSERT_DEBUG(values);
		Quicksort(values, values_size, Forward<CompareFn>(compare));
	}
}  // namespace Orion::Engine::Algorithm
