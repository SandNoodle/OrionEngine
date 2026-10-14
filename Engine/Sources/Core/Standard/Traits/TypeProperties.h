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

	/// @brief Verifies if a given type is constructible (see C++ standard).
	template <typename T, typename... Args>
	inline constexpr bool IsConstructible = Platform::Compiler::IsConstructible<T, Args...>;

	/// @brief Verifies if a given type is copy constructible (see C++ standard).
	template <typename T>
	inline constexpr bool IsCopyConstructible = IsConstructible<T, AddLValueReference<AddConst<T>>>;

	/// @brief Verifies if a given type is move constructible (see C++ standard).
	template <typename T>
	inline constexpr bool IsMoveConstructible = IsConstructible<T, AddRValueReference<AddConst<T>>>;

	/// @brief Verifies if a given type is zero constructible, i.e. initialization by a zero is a valid operation.
	template <typename T>
	inline constexpr bool IsZeroConstructible = EnumKind<T> || ArithmeticKind<T> || IsPointer<T>;

	/// @brief Verifies if a given type is bitwise comparable, i.e. individual bits can be compared in a meaningful way.
	template <typename T>
	inline constexpr bool IsBitwiseComparable = BooleanKind<T> || IntegerKind<T>;

	template <typename T, typename U>
	inline constexpr bool IsAssignable = Platform::Compiler::IsAssignable<T, U>;

	/// @brief Verifies if a given type is cope assignable (see C++ standard).
	template <typename T>
	inline constexpr bool IsCopyAssignable = IsAssignable<T, AddLValueReference<AddConst<T>>>;

	/// @brief Verifies if a given type is move assignable (see C++ standard).
	template <typename T>
	inline constexpr bool IsMoveAssignable = IsAssignable<T, AddRValueReference<AddConst<T>>>;
	// NOLINTEND(readability-identifier-naming)
}  // namespace Orion::Engine
