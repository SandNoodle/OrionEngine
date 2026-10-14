#pragma once

#include "OrionEngine.h"

namespace Orion::Engine
{
	// NOLINTBEGIN(readability-identifier-naming)
	/// @brief Compares two types T and U, evaluating to `true` if both of them name the same type (taking into account
	/// const volatile qualifications).
	template <typename T, typename U>
	inline constexpr bool IsSame = false;

	template <typename T>
	inline constexpr bool IsSame<T, T> = true;
	// NOLINTEND(readability-identifier-naming)

	/// @brief Concept that is satisfied if and only if types T and U denote the same type.
	template <typename T, typename U>
	concept SameAs = IsSame<T, U> && IsSame<U, T>;
}  // namespace Orion::Engine
