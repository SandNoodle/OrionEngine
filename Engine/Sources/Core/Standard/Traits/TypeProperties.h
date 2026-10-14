#pragma once

#include "Core/Standard/Traits/FundamentalTypes.h"
#include "Core/Standard/Traits/TypeQualifiers.h"
#include "Platform/Compiler.h"

namespace Orion::Engine
{
	// NOLINTBEGIN(readability-identifier-naming)
	/// @brief Verifies if a given type is trivially constructible (see C++ standard).
	template <typename T>
	inline constexpr bool IsTriviallyConstructible = Platform::Compiler::IsTriviallyDestructible<T>;

	/// @brief Verifies if a given type is trivially copyable (see C++ standard).
	template <class T>
	inline constexpr bool IsTriviallyCopyable = Platform::Compiler::IsTriviallyCopyable<T>;

	/// @brief Verifies if a given type is trivially destructible (see C++ standard).
	template <class T>
	inline constexpr bool IsTriviallyDestructible = Platform::Compiler::IsTriviallyDestructible<T>;

	/// @brief Type is zero constructible if TODO
	template <typename T>
	inline constexpr bool IsZeroConstructible = EnumKind<T> || ArithmeticKind<T> || IsPointer<T>;

	/// @brief Type is bitwise-comparable if TODO
	template <typename T>
	inline constexpr bool IsBitwiseComparable = BooleanKind<T> || IntegerKind<T>;
	// NOLINTEND(readability-identifier-naming)
}  // namespace Orion::Engine
