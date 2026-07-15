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
		template <typename T> struct RemovePointer { using Type = T; };
		template <typename T> struct RemovePointer<T*> { using Type = T; };
		template <typename T> struct RemovePointer<const T*> { using Type = T; };
		template <typename T> struct RemovePointer<volatile T*> { using Type = T; };
		template <typename T> struct RemovePointer<const volatile T*> { using Type = T; };
		// clang-format on
	}  // namespace Detail

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

	template <typename T>
	using RemoveReference = Detail::RemoveReference<T>::Type;

	template <typename T>
	using RemoveConst = Detail::RemoveConst<T>::Type;

	template <typename T>
	using RemoveVolatile = Detail::RemoveVolatile<T>::Type;

	template <typename T>
	using RemovePointer = Detail::RemovePointer<T>::Type;

	// NOLINTBEGIN(readability-identifier-naming)
	template <typename T, typename U>
	inline constexpr bool IsSame = false;

	template <typename T>
	inline constexpr bool IsSame<T, T> = true;

	template <class T>
	inline constexpr bool IsLValueReference = false;

	template <class T>
	inline constexpr bool IsLValueReference<T&> = true;

	template <class T>
	inline constexpr bool IsRValueReference = false;

	template <class T>
	inline constexpr bool IsRValueReference<T&&> = true;

#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
	template <class T>
	inline constexpr bool IsTriviallyCopyable = __is_trivially_copyable(T);
#else
#error "IsTriviallyCopyable is not defined for this compiler."
#endif

#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
	template <class T>
	inline constexpr bool IsTriviallyDestructible = __is_trivially_destructible(T);
#else
#error "IsTriviallyDestructible is not defined for this compiler."
#endif

#if defined(ORION_COMPILER_CLANG)
	template <typename T>
	inline constexpr bool IsFloatingPoint = __is_floating_point(T);
#else
	namespace Detail
	{
		template <typename T>
		inline constexpr bool IsFloatingPoint = false;

		template <>
		inline constexpr bool IsFloatingPoint<Float32> = true;

		template <>
		inline constexpr bool IsFloatingPoint<Float64> = true;
	}  // namespace Detail

	template <typename T>
	inline constexpr bool IsFloatingPoint = Detail::IsFloatingPoint<T>;
#endif

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
