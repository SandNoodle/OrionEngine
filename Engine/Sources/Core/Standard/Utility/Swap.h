#pragma once

#include "MoveAndForward.h"

namespace Orion::Engine
{
	/// @brief Swaps values of two elements, i.e. A becomes B and B becomes A.
	template <typename T>
	constexpr void Swap(T& lhs, T& rhs) noexcept
	{
		T temp = Move(lhs);
		lhs    = Move(rhs);
		rhs    = Move(temp);
	}
}  // namespace Orion::Engine
