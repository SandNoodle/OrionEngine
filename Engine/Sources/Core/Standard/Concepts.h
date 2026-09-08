#pragma once

#include "OrionEngine.h"

#include "Core/Standard/TypeTraits.h"

namespace Orion::Engine
{
	/// @brief Concept that is satisfied if and only if types T and U denote the same type.
	template <typename T, typename U>
	concept SameAs = IsSame<T, U> && IsSame<U, T>;
}  // namespace Orion::Engine
