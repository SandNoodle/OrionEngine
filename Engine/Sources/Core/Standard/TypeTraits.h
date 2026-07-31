#pragma once

#include "OrionEngine.h"

namespace Orion::Engine
{
	namespace Detail
	{
		// clang-format off
		template <typename T> struct RemoveReference { using Type = T; };
		template <typename T> struct RemoveReference<T&> { using Type = T; };
		template <typename T> struct RemoveReference<const T&> { using Type = T; };
		// clang-format on

		// clang-format off
		template <typename T> struct RemoveConst { using Type = T; };
		template <typename T> struct RemoveConst<const T> { using Type = T; };
		// clang-format on

		// clang-format off
		template <typename T> struct RemoveVolatile { using Type = T; };
		template <typename T> struct RemoveVolatile<volatile T> { using Type = T; };
		// clang-format on

		// clang-format off
		template <typename T> struct RemoveConstVolatile { using Type = T; };
		template <typename T> struct RemoveConstVolatile<const T> { using Type = T; };
		template <typename T> struct RemoveConstVolatile<volatile T> { using Type = T; };
		template <typename T> struct RemoveConstVolatile<const volatile T> { using Type = T; };
		// clang-format on

		// clang-format off
		template <typename T> struct RemovePointer { using Type = T; };
		template <typename T> struct RemovePointer<T*> { using Type = T; };
		template <typename T> struct RemovePointer<const T*> { using Type = T; };
		template <typename T> struct RemovePointer<volatile T*> { using Type = T; };
		template <typename T> struct RemovePointer<const volatile T*> { using Type = T; };
		// clang-format on

		// clang-format off
		template <typename T> struct AddConst { using Type = const T; };
		template <typename T> struct AddVolatile { using Type = volatile T; };
		template <typename T> struct AddConstVolatile { using Type = const volatile T; };
		// clang-format on
	}  // namespace Detail

	/// @brief IntegralConstant is a thin wrapper for a static constant of a given type.
	template <class T, T Value>
	struct IntegralConstant
	{
		using ValueType = T;
		using Type      = IntegralConstant;

		static constexpr ValueType k_value = Value;

		constexpr operator ValueType() const
		{
			return k_value;
		}
		constexpr ValueType operator()() const
		{
			return k_value;
		}
	};
	using TrueType  = IntegralConstant<bool, true>;
	using FalseType = IntegralConstant<bool, false>;

	/// @brief Attempts to remove a reference qualifier from a given type (if present).
	template <typename T>
	using RemoveReference = Detail::RemoveReference<T>::Type;

	/// @brief Attempts to remove a const qualifier from a given type (if present).
	template <typename T>
	using RemoveConst = Detail::RemoveConst<T>::Type;

	/// @brief Attempts to remove a volatile qualifier from a given type (if present).
	template <typename T>
	using RemoveVolatile = Detail::RemoveVolatile<T>::Type;

	/// @brief Attempts to remove both const and volatile qualifiers from a type (if present).
	template <typename T>
	using RemoveConstVolatile = Detail::RemoveConstVolatile<T>::Type;

	/// @brief Attempts to remove const, volatile and reference qualifiers from a type (if present).
	template <typename T>
	using RemoveConstVolatileReference = RemoveConstVolatile<RemoveReference<T>>;

	/// @brief Attempts to add const qualifier to a type (if possible).
	template <typename T>
	using AddConst = Detail::AddConst<T>::Type;

	/// @brief Attempts to add volatile qualifier to a type (if possible).
	template <typename T>
	using AddVolatile = Detail::AddVolatile<T>::Type;

	/// @brief Attempts to add both const and volatile qualifiers to a type (if possible).
	template <typename T>
	using AddConstVolatile = Detail::AddConstVolatile<T>::Type;

	/// @brief Attempts to remove a pointer from a type (if present).
	template <typename T>
	using RemovePointer = Detail::RemovePointer<T>::Type;

	// NOLINTBEGIN(readability-identifier-naming)
	/// @brief Compares two types T and U, evaluating to `true` if both of them name the same type (taking into account
	/// const volatile qualifications).
	template <typename T, typename U>
	inline constexpr bool IsSame = false;

	template <typename T>
	inline constexpr bool IsSame<T, T> = true;

	/// @brief Checks if a given type is an L-Value reference, evaluating to `true` if it is.
	template <class T>
	inline constexpr bool IsLValueReference = false;

	template <class T>
	inline constexpr bool IsLValueReference<T&> = true;

	/// @brief Checks if a given type is an R-Value reference, evaluating to `true` if it is.
	template <class T>
	inline constexpr bool IsRValueReference = false;

	template <class T>
	inline constexpr bool IsRValueReference<T&&> = true;

	/// @brief Verifies if a given type is trivially constructible (see C++ standard).
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
	template <typename T>
	inline constexpr bool IsTriviallyConstructible = __is_trivially_constructible(T);
#else
#error "IsTriviallyConstructible is not defined for this compiler."
#endif

	/// @brief Verifies if a given type is trivially copyable (see C++ standard).
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
	template <class T>
	inline constexpr bool IsTriviallyCopyable = __is_trivially_copyable(T);
#else
#error "IsTriviallyCopyable is not defined for this compiler."
#endif

	/// @brief Verifies if a given type is trivially destructible (see C++ standard).
#if defined(ORION_COMPILER_CLANG)
	template <class T>
	inline constexpr bool IsTriviallyDestructible = __is_trivially_destructible(T);
#elif defined(ORION_COMPILER_GCC)
#if __has_builtin(__is_trivially_destructible)
	template <class T>
	inline constexpr bool IsTriviallyDestructible = __is_trivially_destructible(T);
#elif __has_builtin(__has_trivial_destructor)
	template <class T>
	inline constexpr bool IsTriviallyDestructible = __has_trivial_destructor(T);
#else
#error "__is_trivially_destructible nor __has_trivial_destructor is available for this version of the GCC compiler."
#endif

#else
#error "IsTriviallyDestructible is not defined for this compiler."
#endif

#if defined(ORION_COMPILER_CLANG)
	template <typename T>
	inline constexpr bool IsFloatingPoint = __is_floating_point(T);
#else
	namespace Detail
	{
		// clang-format off
		template <typename T> inline constexpr bool IsFloatingPoint = false;
		template <> inline constexpr bool IsFloatingPoint<Float32>  = true;
		template <> inline constexpr bool IsFloatingPoint<Float64>  = true;
		// clang-format on
	}  // namespace Detail

	/// @brief Checks if a given type is of floating-point kind, evaluating to `true` if it is.
	template <typename T>
	inline constexpr bool IsFloatingPoint = Detail::IsFloatingPoint<T>;
#endif

	namespace Detail
	{
		// clang-format off
		template <typename T> inline constexpr bool IsPointer                    = false;
		template <typename T> inline constexpr bool IsPointer<T*>                = true;
		template <typename T> inline constexpr bool IsPointer<const T*>          = true;
		template <typename T> inline constexpr bool IsPointer<volatile T*>       = true;
		template <typename T> inline constexpr bool IsPointer<const volatile T*> = true;
		// clang-format on
	}  // namespace Detail

	template <typename T>
	inline constexpr bool IsPointer = Detail::IsPointer<T>;

	template <typename T>
	inline constexpr bool IsArithmetic = false;

	// clang-format off
	template <> inline constexpr bool IsArithmetic<bool>  = true;
	template <> inline constexpr bool IsArithmetic<UInt8>  = true;
	template <> inline constexpr bool IsArithmetic<UInt16>  = true;
	template <> inline constexpr bool IsArithmetic<UInt32>  = true;
	template <> inline constexpr bool IsArithmetic<UInt64>  = true;
	template <> inline constexpr bool IsArithmetic<Int8>  = true;
	template <> inline constexpr bool IsArithmetic<Int16>  = true;
	template <> inline constexpr bool IsArithmetic<Int32>  = true;
	template <> inline constexpr bool IsArithmetic<Int64>  = true;
	template <> inline constexpr bool IsArithmetic<Float32>  = true;
	template <> inline constexpr bool IsArithmetic<Float64>  = true;
	template <> inline constexpr bool IsArithmetic<UInt128>  = true;
	template <> inline constexpr bool IsArithmetic<Int128>  = true;

	template <typename T> inline constexpr bool IsArithmetic<const T> = IsArithmetic<T>;
	template <typename T> inline constexpr bool IsArithmetic<volatile T> = IsArithmetic<T>;
	template <typename T> inline constexpr bool IsArithmetic<const volatile T> = IsArithmetic<T>;
	// clang-format on

#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
	template <typename T>
	inline constexpr bool IsEnum = __is_enum(T);
#else
#error "IsEnum is not defined for this compiler."
#endif

#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
	template <typename T>
	inline constexpr bool IsScopedEnum = __is_scoped_enum(T);
#else
#error "IsScopedEnum is not defined for this compiler."
#endif

	/**
	 * @brief Type is zero-constructible if
	 * @tparam T
	 */
	template <typename T>
	inline constexpr bool IsZeroConstructible = IsEnum<T> || IsArithmetic<T> || IsPointer<T>;

#if defined(ORION_COMPILER_CLANG)
	template <typename T>
		requires(IsEnum<T>)
	using UnderlyingType = __underlying_type(T);
#elif defined(ORION_COMPILER_GCC)
	namespace Detail
	{
		template <typename T>
		struct UnderlyingType
		{
			using Type = __underlying_type(T);
		};
	}  // namespace Detail

	template <typename T>
		requires(IsEnum<T>)
	using UnderlyingType = Detail::UnderlyingType<T>::Type;
#else
#error "UnderlyingType is not defined for this compiler."
#endif
	// NOLINTEND(readability-identifier-naming)

	template <typename EnumType>
	constexpr UnderlyingType<EnumType> ToUnderlyingType(EnumType value) noexcept
	{
		return static_cast<UnderlyingType<EnumType>>(value);
	}

	template <typename To, typename From>
	[[nodiscard]] ORION_FORCE_INLINE constexpr To BitCast(const From& v) noexcept
	{
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
		return __builtin_bit_cast(To, v);
#else
#error "BitCast is not defined for this compiler."
#endif
	}
}  // namespace Orion::Engine
