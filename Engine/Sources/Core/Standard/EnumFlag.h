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

	/// @brief Checks if \p flags has all of the \p requested_flags set.
	template <typename Enum>
		requires(IsEnum<Enum> || IsScopedEnum<Enum>)
	[[nodiscard]] ORION_FORCE_INLINE Bool8 EnumHasAllFlags(Enum flags, Enum requested_flags) noexcept
	{
		return ToUnderlyingType(flags & requested_flags) == ToUnderlyingType(requested_flags);
	}

	/// @brief Checks if \p flags contain any of the \p requested_flags.
	template <typename Enum>
		requires(IsEnum<Enum> || IsScopedEnum<Enum>)
	[[nodiscard]] ORION_FORCE_INLINE Bool8 EnumHasAnyFlags(Enum flags, Enum requested_flags) noexcept
	{
		return ToUnderlyingType(flags & requested_flags) != 0;
	}

	/// @brief Checks if \p flags has all of the \p requested_flags set.
	template <typename BitFlag>
	[[nodiscard]] ORION_FORCE_INLINE Bool8 BitFlagHasAllFlags(BitFlag flags, BitFlag requested_flags) noexcept
	{
		static_assert(!(IsEnum<BitFlag> || IsScopedEnum<BitFlag>),
		              "BitFlagHasAllFlags cannot be used with (scoped) enums - use EnumHasAllFlags instead.");
		return (flags & requested_flags) == requested_flags;
	}

	/// @brief Checks if \p flags contain any of the \p requested_flags.
	template <typename BitFlag>
	[[nodiscard]] ORION_FORCE_INLINE Bool8 BitFlagHasAnyFlags(BitFlag flags, BitFlag requested_flags) noexcept
	{
		static_assert(!(IsEnum<BitFlag> || IsScopedEnum<BitFlag>),
		              "BitFlagHasAnyFlags cannot be used with (scoped) enums - use EnumHasAnyFlags instead.");
		return (flags & requested_flags) != 0;
	}

}  // namespace Orion::Engine
