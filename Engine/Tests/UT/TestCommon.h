#pragma once

#include <gtest/gtest.h>

#include "OrionEngine.h"

#include "Core/Standard/Algorithms/Hash.h"

#include <ostream>

namespace Orion::Engine::UT
{
	using PrimitiveTypes = ::testing::Types<Bool8,
	                                        Bool16,
	                                        Bool32,
	                                        Bool64,
	                                        UInt8,
	                                        UInt16,
	                                        UInt32,
	                                        UInt64,
	                                        Int8,
	                                        Int16,
	                                        Int32,
	                                        Int64,
	                                        Char8,
	                                        Char16,
	                                        Char32,
	                                        Float32,
	                                        Float64,
	                                        USize,
	                                        ISize>;

	struct ComplexType
	{
		public:
		using ValueType = Int32;

		public:
		ValueType x{ 0 };

		public:
		constexpr ComplexType() = default;
		constexpr explicit ComplexType(ValueType x);
		virtual ~ComplexType() = default;

#define ORION_COMPLEX_TYPE_ARITHMETIC_OPERATOR_LIST \
	ORION_OPERATOR(+=)                              \
	ORION_OPERATOR(-=)                              \
	ORION_OPERATOR(*=)                              \
	ORION_OPERATOR(/=)

#define ORION_OPERATOR(op)                                            \
	constexpr ComplexType& operator op(const ComplexType& c) noexcept \
	{                                                                 \
		x op c.x;                                                     \
		return *this;                                                 \
	}
		ORION_COMPLEX_TYPE_ARITHMETIC_OPERATOR_LIST
#undef ORION_OPERATOR
#undef ORION_COMPLEX_TYPE_ARITHMETIC_OPERATOR_LIST
	};  // namespace Orion::Engine::UT

	constexpr ComplexType::ComplexType(ValueType x) : x(x) {}

#define ORION_COMPLEX_TYPE_COMPARISON_OPERATOR_LIST \
	ORION_OPERATOR(==)                              \
	ORION_OPERATOR(!=)                              \
	ORION_OPERATOR(<)                               \
	ORION_OPERATOR(<=)                              \
	ORION_OPERATOR(>)                               \
	ORION_OPERATOR(>=)

#define ORION_OPERATOR(op)                                                                            \
	[[nodiscard]] constexpr bool operator op(const ComplexType& lhs, const ComplexType& rhs) noexcept \
	{                                                                                                 \
		return lhs.x op rhs.x;                                                                        \
	}
	ORION_COMPLEX_TYPE_COMPARISON_OPERATOR_LIST
#undef ORION_OPERATOR
#undef ORION_COMPLEX_TYPE_COMPARISON_OPERATOR_LIST

#define ORION_COMPLEX_TYPE_ARITHMETIC_OPERATOR_LIST \
	ORION_OPERATOR(+)                               \
	ORION_OPERATOR(-)                               \
	ORION_OPERATOR(*)                               \
	ORION_OPERATOR(/)

#define ORION_OPERATOR(op)                                                                                   \
	[[nodiscard]] constexpr ComplexType operator op(const ComplexType& lhs, const ComplexType& rhs) noexcept \
	{                                                                                                        \
		return ComplexType(lhs.x op rhs.x);                                                                  \
	}
	ORION_COMPLEX_TYPE_ARITHMETIC_OPERATOR_LIST
#undef ORION_OPERATOR
#undef ORION_COMPLEX_TYPE_ARITHMETIC_OPERATOR_LIST

	inline std::ostream& operator<<(std::ostream& os, const ComplexType& c)
	{
		return os << '[' << c.x << ']';
	}
}  // namespace Orion::Engine::UT

template <>
struct Orion::Engine::Hash<Orion::Engine::UT::ComplexType>
{
	public:
	using ValueType = UT::ComplexType;
	using SizeType  = USize;

	public:
	SizeType operator()(const ValueType& v)
	{
		return static_cast<SizeType>(v.x);
	}
};  // namespace Orion::Engine
