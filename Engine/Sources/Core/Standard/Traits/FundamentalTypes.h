#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Traits/TypeComparison.h"
#include "Core/Standard/Traits/TypeQualifiers.h"
#include "Platform/Compiler.h"

namespace Orion::Engine
{
	namespace Detail
	{
		template <typename T>
		concept VoidKind = SameAs<T, void>;

		template <typename T>
		concept ByteKind = SameAs<T, Byte>;

		template <typename T>
		concept BooleanKind = SameAs<T, bool>    //
		                   || SameAs<T, Bool8>   //
		                   || SameAs<T, Bool16>  //
		                   || SameAs<T, Bool32>  //
		                   || SameAs<T, Bool64>  //
			;

		template <typename T>
		concept CharacterKind = SameAs<T, Char8>   //
		                     || SameAs<T, Char16>  //
		                     || SameAs<T, Char32>  //
			;

		template <typename T>
		concept FloatingPointKind = SameAs<T, Float32>  //
		                         || SameAs<T, Float64>  //
			;

		template <typename T>
		concept SignedIntegerKind = SameAs<T, Int8>    //
		                         || SameAs<T, Int16>   //
		                         || SameAs<T, Int32>   //
		                         || SameAs<T, Int64>   //
		                         || SameAs<T, Int128>  //
			;

		template <typename T>
		concept UnsignedIntegerKind = SameAs<T, UInt8>    //
		                           || SameAs<T, UInt16>   //
		                           || SameAs<T, UInt32>   //
		                           || SameAs<T, UInt64>   //
		                           || SameAs<T, UInt128>  //
			;

		template <typename T>
		concept IntegerKind = SignedIntegerKind<T>    //
		                   || UnsignedIntegerKind<T>  //
			;
	}  // namespace Detail

	/// @brief Concept that is satisfied by C-styled enum and C++ enum class types.
	template <typename T>
	concept EnumKind = Platform::Compiler::IsEnum<T>;

	/// @brief Concept that is satisfied C++ enum class types.
	template <typename T>
	concept ScopedEnumKind = Platform::Compiler::IsScopedEnum<T>;

	/// @brief Provides a typedef for the underlying type making up an enum.
	template <EnumKind T>
	using EnumUnderlyingType = Platform::Compiler::UnderlyingType<T>;

	/// @brief Converts an enumeration into its underlying type's \p value.
	template <EnumKind T>
	constexpr EnumUnderlyingType<T> EnumAsUnderlyingType(T value) noexcept
	{
		return static_cast<EnumUnderlyingType<T>>(value);
	}

	/// @brief Concept that is satisfied by a void type (possibly CV-qualified).
	template <typename T>
	concept VoidKind = Detail::VoidKind<RemoveConstVolatile<T>>;

	/// @brief Concept that is satisfied by a byte types (possibly CV-qualified).
	template <typename T>
	concept ByteKind = Detail::ByteKind<RemoveConstVolatile<T>>;

	/// @brief Concept that is satisfied by a boolean types (possibly CV-qualified).
	template <typename T>
	concept BooleanKind = Detail::BooleanKind<RemoveConstVolatile<T>>;

	/// @brief Concept that is satisfied by a character types (possibly CV-qualified).
	template <typename T>
	concept CharacterKind = Detail::CharacterKind<RemoveConstVolatile<T>>;

	/// @brief Concept that is satisfied by a floating point types (possibly CV-qualified).
	template <typename T>
	concept FloatingPointKind = Detail::FloatingPointKind<RemoveConstVolatile<T>>;

	/// @brief Concept that is satisfied by a signed integer types (possibly CV-qualified).
	template <typename T>
	concept SignedIntegerKind = Detail::SignedIntegerKind<RemoveConstVolatile<T>>;

	/// @brief Concept that is satisfied by a unsigned integer types (possibly CV-qualified).
	template <typename T>
	concept UnsignedIntegerKind = Detail::UnsignedIntegerKind<RemoveConstVolatile<T>>;

	/// @brief Concept that is satisfied by a integer types (both signed and unsigned; possibly CV-qualified).
	template <typename T>
	concept IntegerKind = SignedIntegerKind<T>    //
	                   || UnsignedIntegerKind<T>  //
		;

	/// @brief Concept that is satisfied for C++ integral types (possibly CV-qualified).
	/// @see https://en.cppreference.com/cpp/language/types#Integral_types
	template <typename T>
	concept IntegralKind = BooleanKind<T>    //
	                    || CharacterKind<T>  //
	                    || IntegerKind<T>    //
		;

	/// @brief Concept that is satisfied for C++ arithmetic types (possibly CV-qualified).
	/// @see https://en.cppreference.com/c/language/arithmetic_types
	template <typename T>
	concept ArithmeticKind = BooleanKind<T>        //
	                      || CharacterKind<T>      //
	                      || IntegerKind<T>        //
	                      || FloatingPointKind<T>  //
		;

	/// @brief Concept that is satisfied for C++ fundamental types (possibly CV-qualified).
	/// @see https://en.cppreference.com/cpp/language/types
	template <typename T>
	concept FundamentalKind = VoidKind<T>           //
	                       || ByteKind<T>           //
	                       || BooleanKind<T>        //
	                       || CharacterKind<T>      //
	                       || FloatingPointKind<T>  //
	                       || IntegerKind<T>        //
	                       || EnumKind<T>           //
	                       || ScopedEnumKind<T>     //
		;
}  // namespace Orion::Engine
