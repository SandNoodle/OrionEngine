#pragma once

#include "Core/Assert.h"
#include "Core/Standard/Algorithms/Compare.h"
#include "Core/Standard/Utility/MoveAndForward.h"
#include "Core/Standard/Utility/Swap.h"

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

	template <typename T, typename CompareFn>
	constexpr void Quicksort(T* values, USize values_size, CompareFn&& compare = Less<T>)
	{
		ORION_ASSERT_DEBUG(values);
		Internal::Quicksort(values, 0, values_size - 1, Forward<CompareFn>(compare));
	}

	template <typename T, typename CompareFn>
	constexpr void Sort(T* values, USize values_size, CompareFn&& compare = Less<T>)
	{
		ORION_ASSERT_DEBUG(values);
		Quicksort(values, values_size, Forward<CompareFn>(compare));
	}
}  // namespace Orion::Engine::Algorithm
