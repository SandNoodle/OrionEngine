#pragma once

#include "OrionEngine.h"

#include "Core/Standard/TypeTraits.h"

namespace Orion::Engine
{
	/// @brief TODO
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr RemoveReference<T>&& Move(T&& t) noexcept
	{
		return static_cast<RemoveReference<T>&&>(t);
	}

	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T&& Forward(RemoveReference<T>& t) noexcept
	{
		return static_cast<T&&>(t);
	}

	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T&& Forward(RemoveReference<T>&& t) noexcept
	{
		static_assert(!IsLValueReference<T>);
		return static_cast<T&&>(t);
	}
}  // namespace Orion::Engine
