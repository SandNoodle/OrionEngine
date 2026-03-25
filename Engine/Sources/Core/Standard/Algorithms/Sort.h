#pragma once

#include "Core/Standard/MoveAndForward.h"
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
					index++;
					Swap(pivot_index, index);
				}
			}
			return pivot_index;
		}

		template <typename T, typename CompareFn>
		constexpr void Quicksort(T* values, USize lhs, USize rhs, CompareFn&& compare) noexcept
		{
			if (lhs >= rhs) {
				return;
			}
			USize pivot_index = QuicksortPartition(values, lhs, rhs, Forward<CompareFn>(compare));
			Quicksort(values, lhs, pivot_index - 1, Forward<CompareFn>(compare));
			Quicksort(values, pivot_index + 1, rhs, Forward<CompareFn>(compare));
		}
	}  // namespace Internal

	template <typename T, typename CompareFn>
	constexpr void Quicksort(T* values, USize values_size, CompareFn&& compare)
	{
		Internal::Quicksort(values, 0, values_size - 1, Forward<CompareFn>(compare));
	}

	template <typename T, typename CompareFn>
	constexpr void Sort(T* values, USize values_size, CompareFn&& compare)
	{
		Quicksort(values, values_size, Forward<CompareFn>(compare));
	}
}  // namespace Orion::Engine::Algorithm
