#include "TestCommon.h"

#include "Core/Standard/Containers/Result.h"
#include "Core/Standard/Containers/String.h"

namespace Orion::Engine::UT
{
	static String k_error_message = ORION_STRING("Some error message");

	namespace
	{
		template <typename T>
		class ResultTest : public ::testing::Test
		{
		};
	}  // namespace
	TYPED_TEST_SUITE_P(ResultTest);

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

	TYPED_TEST_P(ResultTest, Constructor_Default)
	{
		Result<TypeParam> r{};
		EXPECT_TRUE(r.IsValue());
		EXPECT_FALSE(r.IsError());
		EXPECT_EQ(r.Value(), TypeParam{});
		EXPECT_EQ(*r, TypeParam{});
	}

	TYPED_TEST_P(ResultTest, Constructor_Value_ByCopy)
	{
		TypeParam v = Value<TypeParam>(1);

		Result<TypeParam> r{ v };
		EXPECT_TRUE(r.IsValue());
		EXPECT_FALSE(r.IsError());
		EXPECT_EQ(r.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r, Value<TypeParam>(1));
	}

	TYPED_TEST_P(ResultTest, Constructor_Value_ByMove)
	{
		TypeParam v = Value<TypeParam>(1);

		Result<TypeParam> r{ Move(v) };
		EXPECT_TRUE(r.IsValue());
		EXPECT_FALSE(r.IsError());
		EXPECT_EQ(r.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r, Value<TypeParam>(1));
	}

	TYPED_TEST_P(ResultTest, Constructor_Error_ByCopy)
	{
		Error error(k_error_message);

		Result<TypeParam> r{ error };
		EXPECT_FALSE(r.IsValue());
		EXPECT_TRUE(r.IsError());
		EXPECT_EQ(r.Error(), Error(k_error_message));
	}

	TYPED_TEST_P(ResultTest, Constructor_Error_ByMove)
	{
		Error error(k_error_message);

		Result<TypeParam> r{ Move(error) };
		EXPECT_FALSE(r.IsValue());
		EXPECT_TRUE(r.IsError());
		EXPECT_EQ(r.Error(), Error(k_error_message));
	}

	TYPED_TEST_P(ResultTest, Constructor_Copy)
	{
		TypeParam v = Value<TypeParam>(1);

		Result<TypeParam> r1{ v };
		EXPECT_TRUE(r1.IsValue());
		EXPECT_FALSE(r1.IsError());
		EXPECT_EQ(r1.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r1, Value<TypeParam>(1));

		Result<TypeParam> r2(r1);
		EXPECT_TRUE(r2.IsValue());
		EXPECT_FALSE(r2.IsError());
		EXPECT_EQ(r2.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r2, Value<TypeParam>(1));
	}

	TYPED_TEST_P(ResultTest, Constructor_Move)
	{
		TypeParam v = Value<TypeParam>(1);

		Result<TypeParam> r1{ v };
		EXPECT_TRUE(r1.IsValue());
		EXPECT_FALSE(r1.IsError());
		EXPECT_EQ(r1.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r1, Value<TypeParam>(1));

		Result<TypeParam> r2(Move(r1));
		EXPECT_TRUE(r2.IsValue());
		EXPECT_FALSE(r2.IsError());
		EXPECT_EQ(r2.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r2, Value<TypeParam>(1));
	}

	TYPED_TEST_P(ResultTest, Operator_Value_ByCopy)
	{
		Error error(k_error_message);

		Result<TypeParam> r{ error };
		EXPECT_FALSE(r.IsValue());
		EXPECT_TRUE(r.IsError());
		EXPECT_EQ(r.Error(), Error(k_error_message));

		TypeParam v = Value<TypeParam>(1);

		r = v;
		EXPECT_TRUE(r.IsValue());
		EXPECT_FALSE(r.IsError());
		EXPECT_EQ(r.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r, Value<TypeParam>(1));
	}

	TYPED_TEST_P(ResultTest, Operator_Value_ByMove)
	{
		Error error(k_error_message);

		Result<TypeParam> r{ error };
		EXPECT_FALSE(r.IsValue());
		EXPECT_TRUE(r.IsError());
		EXPECT_EQ(r.Error(), Error(k_error_message));

		TypeParam v = Value<TypeParam>(1);

		r = Move(v);
		EXPECT_TRUE(r.IsValue());
		EXPECT_FALSE(r.IsError());
		EXPECT_EQ(r.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r, Value<TypeParam>(1));
	}

	TYPED_TEST_P(ResultTest, Operator_Error_ByCopy)
	{
		TypeParam v = Value<TypeParam>(1);

		Result<TypeParam> r{ v };
		EXPECT_TRUE(r.IsValue());
		EXPECT_FALSE(r.IsError());
		EXPECT_EQ(r.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r, Value<TypeParam>(1));

		Error error(k_error_message);

		r = error;
		EXPECT_FALSE(r.IsValue());
		EXPECT_TRUE(r.IsError());
		EXPECT_EQ(r.Error(), Error(k_error_message));
	}

	TYPED_TEST_P(ResultTest, Operator_Error_ByMove)
	{
		TypeParam v = Value<TypeParam>(1);

		Result<TypeParam> r{ v };
		EXPECT_TRUE(r.IsValue());
		EXPECT_FALSE(r.IsError());
		EXPECT_EQ(r.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r, Value<TypeParam>(1));

		Error error(k_error_message);

		r = Move(error);
		EXPECT_FALSE(r.IsValue());
		EXPECT_TRUE(r.IsError());
		EXPECT_EQ(r.Error(), Error(k_error_message));
	}

	TYPED_TEST_P(ResultTest, Operator_Copy)
	{
		TypeParam v = Value<TypeParam>(1);

		Result<TypeParam> r1{ v };
		EXPECT_TRUE(r1.IsValue());
		EXPECT_FALSE(r1.IsError());
		EXPECT_EQ(r1.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r1, Value<TypeParam>(1));

		Result<TypeParam> r2 = r1;
		EXPECT_TRUE(r2.IsValue());
		EXPECT_FALSE(r2.IsError());
		EXPECT_EQ(r2.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r2, Value<TypeParam>(1));
	}

	TYPED_TEST_P(ResultTest, Operator_Move)
	{
		TypeParam v = Value<TypeParam>(1);

		Result<TypeParam> r1{ v };
		EXPECT_TRUE(r1.IsValue());
		EXPECT_FALSE(r1.IsError());
		EXPECT_EQ(r1.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r1, Value<TypeParam>(1));

		Result<TypeParam> r2 = Move(r1);
		EXPECT_TRUE(r2.IsValue());
		EXPECT_FALSE(r2.IsError());
		EXPECT_EQ(r2.Value(), Value<TypeParam>(1));
		EXPECT_EQ(*r2, Value<TypeParam>(1));
	}

	TYPED_TEST_P(ResultTest, Operator_Value_ByIndirection)
	{
		Result<TypeParam> r{ Value<TypeParam>(1) };
		EXPECT_TRUE(r.IsValue());
		EXPECT_FALSE(r.IsError());
		EXPECT_EQ(r.operator*(), Value<TypeParam>(1));
	}

	TYPED_TEST_P(ResultTest, Operator_Value_ByPointer)
	{
		Result<TypeParam> r{ Value<TypeParam>(1) };
		EXPECT_TRUE(r.IsValue());
		EXPECT_FALSE(r.IsError());
		EXPECT_EQ(*r.operator->(), Value<TypeParam>(1));
	}

	TYPED_TEST_P(ResultTest, Get_Value)
	{
		GTEST_FAIL();
	}

	TYPED_TEST_P(ResultTest, Get_ValueOr)
	{
		GTEST_FAIL();
	}

	TYPED_TEST_P(ResultTest, Get_ValueOrDefault)
	{
		GTEST_FAIL();
	}

	TYPED_TEST_P(ResultTest, Get_Error)
	{
		GTEST_FAIL();
	}

	REGISTER_TYPED_TEST_SUITE_P(ResultTest,
	                            Constructor_Default,
	                            Constructor_Value_ByCopy,
	                            Constructor_Value_ByMove,
	                            Constructor_Error_ByCopy,
	                            Constructor_Error_ByMove,
	                            Constructor_Copy,
	                            Constructor_Move,
	                            Operator_Value_ByCopy,
	                            Operator_Value_ByMove,
	                            Operator_Error_ByCopy,
	                            Operator_Error_ByMove,
	                            Operator_Copy,
	                            Operator_Move,
	                            Operator_Value_ByIndirection,
	                            Operator_Value_ByPointer,
	                            Get_Value,
	                            Get_ValueOr,
	                            Get_ValueOrDefault,
	                            Get_Error);

	INSTANTIATE_TYPED_TEST_SUITE_P(ResultPrimitiveTypesTest, ResultTest, PrimitiveTypes);
	INSTANTIATE_TYPED_TEST_SUITE_P(ResultComplexTypesTest, ResultTest, ComplexType);
}  // namespace Orion::Engine::UT
