#include "TestCommon.h"

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

	TYPED_TEST_P(OptionalTest, Constructor_Default)
	{
		GTEST_FAIL();
	}

	TYPED_TEST_P(OptionalTest, Constructor_Value_ByCopy)
	{
		GTEST_FAIL();
	}

	TYPED_TEST_P(OptionalTest, Constructor_Value_ByMove)
	{
		GTEST_FAIL();
	}

	TYPED_TEST_P(OptionalTest, Constructor_NullOption)
	{
		GTEST_FAIL();
	}

	TYPED_TEST_P(OptionalTest, Constructor_Copy)
	{
		GTEST_FAIL();
	}

	TYPED_TEST_P(OptionalTest, Constructor_Move)
	{
		GTEST_FAIL();
	}

	TYPED_TEST_P(OptionalTest, Operator_Value_ByCopy)
	{
		GTEST_FAIL();
	}

	TYPED_TEST_P(OptionalTest, Operator_Value_ByMove)
	{
		GTEST_FAIL();
	}

	TYPED_TEST_P(OptionalTest, Operator_NullOption)
	{
		GTEST_FAIL();
	}

	TYPED_TEST_P(OptionalTest, Operator_Copy)
	{
		GTEST_FAIL();
	}

	TYPED_TEST_P(OptionalTest, Operator_Move)
	{
		GTEST_FAIL();
	}

	TYPED_TEST_P(OptionalTest, Operator_Value_ByIndirection)
	{
		GTEST_FAIL();
	}

	TYPED_TEST_P(OptionalTest, Operator_Value_ByPointer)
	{
		GTEST_FAIL();
	}

	TYPED_TEST_P(OptionalTest, Get_Value)
	{
		GTEST_FAIL();
	}

	TYPED_TEST_P(OptionalTest, Get_ValueOr)
	{
		GTEST_FAIL();
	}

	TYPED_TEST_P(OptionalTest, Get_ValueOrDefault)
	{
		GTEST_FAIL();
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
