#pragma once

#include "Core/Standard/Utility/MoveAndForward.h"

namespace Orion::Engine
{
	/// @brief Pair is a type which allows storing two subtypes \tparam T1 and \tparam T2 in a single unit.
	template <typename T1, typename T2>
	struct Pair
	{
		T1 first;
		T2 second;
	};

	// -- Non-member functions.
	template <typename T1, typename T2>
	constexpr Pair<T1, T2> MakePair(T1&& t1, T2&& t2) noexcept
	{
		return Pair<T1, T2>(Move(t1), Move(t2));
	}

	// -- Deduction guides.
	template <class T1, class T2>
	Pair(T1, T2) -> Pair<T1, T2>;

	// -- Implementation.
}  // namespace Orion::Engine
