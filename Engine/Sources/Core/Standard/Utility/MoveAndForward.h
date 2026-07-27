#pragma once

#include "OrionEngine.h"

#include "Core/Standard/TypeTraits.h"

namespace Orion::Engine
{
	/// @brief Function that indicate that the \tparam T resources can be transferred efficiently, i.e. 'moved'
	/// according to the C++ move semantics.
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr RemoveReference<T>&& Move(T&& t) noexcept
	{
		return static_cast<RemoveReference<T>&&>(t);
	}

	/// @brief Function that forwards LValues as either LValues or as RValues (depending on \tparam T)
	/// @{
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T&& Forward(RemoveReference<T>& t) noexcept
	{
		return static_cast<T&&>(t);
	}

	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T&& Forward(RemoveReference<T>&& t) noexcept
	{
		static_assert(!IsLValueReference<T>, "Cannot Forward<T>(...) LValue reference as a RValue.");
		return static_cast<T&&>(t);
	}
	/// @}
}  // namespace Orion::Engine
