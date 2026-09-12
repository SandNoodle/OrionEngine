#include "TestCommon.h"

#include "Core/Standard/Containers/Optional.h"

namespace Orion::Engine::UT
{
	namespace
	{
		template <typename T>
		class OptionalTest : public ::testing::Test
		{
		};
	}  // namespace
	TYPED_TEST_SUITE_P(OptionalTest);

	template <typename T>
	static constexpr T Value(USize index)
	{
		return static_cast<T>(index);
	}

	template <>
	constexpr ComplexType Value<ComplexType>(USize index)
	{
		return ComplexType(static_cast<ComplexType::ValueType>(index));
	}

	TYPED_TEST_P(OptionalTest, Constructor_Default)
	{
		Optional<TypeParam> r{};
		EXPECT_TRUE(r.IsValue());
		EXPECT_FALSE(r.IsNull());
		EXPECT_EQ(r.Value(), TypeParam{});
		EXPECT_EQ(*r, TypeParam{});
	}

	TYPED_TEST_P(OptionalTest, Constructor_Value_ByCopy)
	{
		TypeParam v = Value<TypeParam>(1);

		Optional<TypeParam> r{ v };
		EXPECT_TRUE(r.IsValue());
		EXPECT_FALSE(r.IsNull());
		EXPECT_EQ(r.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r, Value<TypeParam>(1));
	}

	TYPED_TEST_P(OptionalTest, Constructor_Value_ByMove)
	{
		TypeParam v = Value<TypeParam>(1);

		Optional<TypeParam> r{ Move(v) };
		EXPECT_TRUE(r.IsValue());
		EXPECT_FALSE(r.IsNull());
		EXPECT_EQ(r.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r, Value<TypeParam>(1));
	}

	TYPED_TEST_P(OptionalTest, Constructor_NullOption)
	{
		Optional<TypeParam> r{ k_null_option };
		EXPECT_FALSE(r.IsValue());
		EXPECT_TRUE(r.IsNull());
	}

	TYPED_TEST_P(OptionalTest, Constructor_Copy)
	{
		TypeParam v = Value<TypeParam>(1);

		Optional<TypeParam> r1{ v };
		EXPECT_TRUE(r1.IsValue());
		EXPECT_FALSE(r1.IsNull());
		EXPECT_EQ(r1.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r1, Value<TypeParam>(1));

		Optional<TypeParam> r2(r1);

		EXPECT_TRUE(r1.IsValue());
		EXPECT_FALSE(r1.IsNull());
		EXPECT_EQ(r1.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r1, Value<TypeParam>(1));

		EXPECT_TRUE(r2.IsValue());
		EXPECT_FALSE(r2.IsNull());
		EXPECT_EQ(r2.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r2, Value<TypeParam>(1));
	}

	TYPED_TEST_P(OptionalTest, Constructor_Move)
	{
		TypeParam v = Value<TypeParam>(1);

		Optional<TypeParam> r1{ v };
		EXPECT_TRUE(r1.IsValue());
		EXPECT_FALSE(r1.IsNull());
		EXPECT_EQ(r1.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r1, Value<TypeParam>(1));

		Optional<TypeParam> r2(Move(r1));
		EXPECT_TRUE(r2.IsValue());
		EXPECT_FALSE(r2.IsNull());
		EXPECT_EQ(r2.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r2, Value<TypeParam>(1));
	}

	TYPED_TEST_P(OptionalTest, Operator_Value_ByCopy)
	{
		Optional<TypeParam> r{ k_null_option };
		EXPECT_FALSE(r.IsValue());
		EXPECT_TRUE(r.IsNull());

		TypeParam v = Value<TypeParam>(1);

		r = v;
		EXPECT_TRUE(r.IsValue());
		EXPECT_FALSE(r.IsNull());
		EXPECT_EQ(r.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r, Value<TypeParam>(1));
	}

	TYPED_TEST_P(OptionalTest, Operator_Value_ByMove)
	{
		Optional<TypeParam> r{ k_null_option };
		EXPECT_FALSE(r.IsValue());
		EXPECT_TRUE(r.IsNull());

		TypeParam v = Value<TypeParam>(1);

		r = Move(v);
		EXPECT_TRUE(r.IsValue());
		EXPECT_FALSE(r.IsNull());
		EXPECT_EQ(r.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r, Value<TypeParam>(1));
	}

	TYPED_TEST_P(OptionalTest, Operator_NullOption)
	{
		Optional<TypeParam> r{ Value<TypeParam>(1) };
		EXPECT_TRUE(r.IsValue());
		EXPECT_FALSE(r.IsNull());
		EXPECT_EQ(r.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r, Value<TypeParam>(1));

		r = k_null_option;
		EXPECT_FALSE(r.IsValue());
		EXPECT_TRUE(r.IsNull());
	}

	TYPED_TEST_P(OptionalTest, Operator_Copy)
	{
		Optional<TypeParam> r1{ Value<TypeParam>(1) };
		EXPECT_TRUE(r1.IsValue());
		EXPECT_FALSE(r1.IsNull());
		EXPECT_EQ(r1.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r1, Value<TypeParam>(1));

		Optional<TypeParam> r2{ k_null_option };
		EXPECT_FALSE(r2.IsValue());
		EXPECT_TRUE(r2.IsNull());

		r2 = r1;
		EXPECT_TRUE(r1.IsValue());
		EXPECT_FALSE(r1.IsNull());
		EXPECT_EQ(r1.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r1, Value<TypeParam>(1));

		EXPECT_TRUE(r2.IsValue());
		EXPECT_FALSE(r2.IsNull());
		EXPECT_EQ(r2.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r2, Value<TypeParam>(1));
	}

	TYPED_TEST_P(OptionalTest, Operator_Move)
	{
		Optional<TypeParam> r1{ Value<TypeParam>(1) };
		EXPECT_TRUE(r1.IsValue());
		EXPECT_FALSE(r1.IsNull());
		EXPECT_EQ(r1.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r1, Value<TypeParam>(1));

		Optional<TypeParam> r2{ k_null_option };
		EXPECT_FALSE(r2.IsValue());
		EXPECT_TRUE(r2.IsNull());

		r2 = Move(r1);
		EXPECT_TRUE(r2.IsValue());
		EXPECT_FALSE(r2.IsNull());
		EXPECT_EQ(r2.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r2, Value<TypeParam>(1));
	}

	TYPED_TEST_P(OptionalTest, Operator_Value_ByIndirection)
	{
		Optional<TypeParam> r{ Value<TypeParam>(1) };
		EXPECT_TRUE(r.IsValue());
		EXPECT_FALSE(r.IsNull());
		EXPECT_EQ(r.operator*(), Value<TypeParam>(1));
	}

	TYPED_TEST_P(OptionalTest, Operator_Value_ByPointer)
	{
		Optional<TypeParam> r{ Value<TypeParam>(1) };
		EXPECT_TRUE(r.IsValue());
		EXPECT_FALSE(r.IsNull());
		EXPECT_EQ(*r.operator->(), Value<TypeParam>(1));
	}

	TYPED_TEST_P(OptionalTest, Get_Value)
	{
		Optional<TypeParam> r{ Value<TypeParam>(1) };
		EXPECT_TRUE(r.IsValue());
		EXPECT_FALSE(r.IsNull());
		EXPECT_EQ(r.Value(), Value<TypeParam>(1));
	}

	TYPED_TEST_P(OptionalTest, Get_ValueOr)
	{
		Optional<TypeParam> r{ k_null_option };
		EXPECT_FALSE(r.IsValue());
		EXPECT_TRUE(r.IsNull());
		EXPECT_EQ(r.ValueOr(Value<TypeParam>(1)), Value<TypeParam>(1));
	}

	TYPED_TEST_P(OptionalTest, Get_ValueOrDefault)
	{
		Optional<TypeParam> r{ k_null_option };
		EXPECT_FALSE(r.IsValue());
		EXPECT_TRUE(r.IsNull());
		EXPECT_EQ(r.ValueOrDefault(), Value<TypeParam>(0));
	}

	REGISTER_TYPED_TEST_SUITE_P(OptionalTest,
	                            Constructor_Default,
	                            Constructor_Value_ByCopy,
	                            Constructor_Value_ByMove,
	                            Constructor_NullOption,
	                            Constructor_Copy,
	                            Constructor_Move,
	                            Operator_Value_ByCopy,
	                            Operator_Value_ByMove,
	                            Operator_NullOption,
	                            Operator_Copy,
	                            Operator_Move,
	                            Operator_Value_ByIndirection,
	                            Operator_Value_ByPointer,
	                            Get_Value,
	                            Get_ValueOr,
	                            Get_ValueOrDefault);

	INSTANTIATE_TYPED_TEST_SUITE_P(OptionalPrimitiveTypesTest, OptionalTest, PrimitiveTypes);
	INSTANTIATE_TYPED_TEST_SUITE_P(OptionalComplexTypesTest, OptionalTest, ComplexType);
}  // namespace Orion::Engine::UT
