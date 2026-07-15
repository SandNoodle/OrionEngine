#pragma once

#include "OrionEngine.h"

#include "Core/Standard/TypeTraits.h"

namespace Orion::Engine
{
/**
 * @brief Defines all bitwise operations for a given \p Enum class,
 * which enables it to be used as an enum flag.
 */
#define ORION_ENUM_FLAG(Enum)                                                                                     \
	static_assert(IsScopedEnum<Enum>, "Cannot define operator flags for type, because it is not a scoped enum."); \
	ORION_FORCE_INLINE constexpr Enum& operator|=(Enum& lhs, Enum rhs)                                            \
	{                                                                                                             \
		return lhs = static_cast<Enum>(ToUnderlyingType(lhs) | ToUnderlyingType(rhs));                            \
	}                                                                                                             \
	ORION_FORCE_INLINE constexpr Enum& operator&=(Enum& lhs, Enum rhs)                                            \
	{                                                                                                             \
		return lhs = static_cast<Enum>(ToUnderlyingType(lhs) & ToUnderlyingType(rhs));                            \
	}                                                                                                             \
	ORION_FORCE_INLINE constexpr Enum& operator^=(Enum& lhs, Enum rhs)                                            \
	{                                                                                                             \
		return lhs = static_cast<Enum>(ToUnderlyingType(lhs) ^ ToUnderlyingType(rhs));                            \
	}                                                                                                             \
	ORION_FORCE_INLINE constexpr Enum operator|(Enum lhs, Enum rhs)                                               \
	{                                                                                                             \
		return static_cast<Enum>(ToUnderlyingType(lhs) | ToUnderlyingType(rhs));                                  \
	}                                                                                                             \
	ORION_FORCE_INLINE constexpr Enum operator&(Enum lhs, Enum rhs)                                               \
	{                                                                                                             \
		return static_cast<Enum>(ToUnderlyingType(lhs) & ToUnderlyingType(rhs));                                  \
	}                                                                                                             \
	ORION_FORCE_INLINE constexpr Enum operator^(Enum lhs, Enum rhs)                                               \
	{                                                                                                             \
		return static_cast<Enum>(ToUnderlyingType(lhs) ^ ToUnderlyingType(rhs));                                  \
	}                                                                                                             \
	ORION_FORCE_INLINE constexpr Enum operator!(Enum e)                                                           \
	{                                                                                                             \
		return static_cast<Enum>(!ToUnderlyingType(e));                                                           \
	}                                                                                                             \
	ORION_FORCE_INLINE constexpr Enum operator~(Enum e)                                                           \
	{                                                                                                             \
		return static_cast<Enum>(~ToUnderlyingType(e));                                                           \
	}
}  // namespace Orion::Engine
