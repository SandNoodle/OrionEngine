#include "TestCommon.h"

#include "Core/Standard/Algorithms/Compare.h"
#include "Core/Standard/Containers/Array.h"

namespace Orion::Engine::Algorithm::UT
{
	using namespace Orion::Engine::UT;

	namespace
	{
		template <typename T>
		class CompareTest : public ::testing::Test
		{
		};
	}  // namespace
	TYPED_TEST_SUITE_P(CompareTest);

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

	TYPED_TEST_P(CompareTest, Less)
	{
		TypeParam v1 = Value<TypeParam>(0);
		TypeParam v2 = Value<TypeParam>(1);
		TypeParam v3 = Value<TypeParam>(1);
		EXPECT_TRUE(Algorithm::Less(v1, v2));
		EXPECT_TRUE(Algorithm::Less(v1, v3));
		EXPECT_FALSE(Algorithm::Less(v2, v3));
		EXPECT_FALSE(Algorithm::Less(v2, v1));
		EXPECT_FALSE(Algorithm::Less(v3, v1));
		EXPECT_FALSE(Algorithm::Less(v3, v2));
	}

	TYPED_TEST_P(CompareTest, LessEqual)
	{
		TypeParam v1 = Value<TypeParam>(0);
		TypeParam v2 = Value<TypeParam>(1);
		TypeParam v3 = Value<TypeParam>(1);
		EXPECT_TRUE(Algorithm::LessEqual(v1, v2));
		EXPECT_TRUE(Algorithm::LessEqual(v1, v3));
		EXPECT_TRUE(Algorithm::LessEqual(v2, v3));
		EXPECT_FALSE(Algorithm::LessEqual(v2, v1));
		EXPECT_FALSE(Algorithm::LessEqual(v3, v1));
		EXPECT_TRUE(Algorithm::LessEqual(v3, v2));
	}

	TYPED_TEST_P(CompareTest, Greater)
	{
		TypeParam v1 = Value<TypeParam>(0);
		TypeParam v2 = Value<TypeParam>(1);
		TypeParam v3 = Value<TypeParam>(1);
		EXPECT_FALSE(Algorithm::Greater(v1, v2));
		EXPECT_FALSE(Algorithm::Greater(v1, v3));
		EXPECT_FALSE(Algorithm::Greater(v2, v3));
		EXPECT_TRUE(Algorithm::Greater(v2, v1));
		EXPECT_TRUE(Algorithm::Greater(v3, v1));
		EXPECT_FALSE(Algorithm::Greater(v3, v2));
	}

	TYPED_TEST_P(CompareTest, GreaterEqual)
	{
		TypeParam v1 = Value<TypeParam>(0);
		TypeParam v2 = Value<TypeParam>(1);
		TypeParam v3 = Value<TypeParam>(1);
		EXPECT_FALSE(Algorithm::GreaterEqual(v1, v2));
		EXPECT_FALSE(Algorithm::GreaterEqual(v1, v3));
		EXPECT_TRUE(Algorithm::GreaterEqual(v2, v3));
		EXPECT_TRUE(Algorithm::GreaterEqual(v2, v1));
		EXPECT_TRUE(Algorithm::GreaterEqual(v3, v1));
		EXPECT_TRUE(Algorithm::GreaterEqual(v3, v2));
	}

	TYPED_TEST_P(CompareTest, Equal)
	{
		TypeParam v1 = Value<TypeParam>(0);
		TypeParam v2 = Value<TypeParam>(1);
		TypeParam v3 = Value<TypeParam>(1);
		EXPECT_FALSE(Algorithm::Equal(v1, v2));
		EXPECT_FALSE(Algorithm::Equal(v1, v3));
		EXPECT_TRUE(Algorithm::Equal(v2, v3));
		EXPECT_FALSE(Algorithm::Equal(v2, v1));
		EXPECT_FALSE(Algorithm::Equal(v3, v1));
		EXPECT_TRUE(Algorithm::Equal(v3, v2));
	}

	TYPED_TEST_P(CompareTest, NotEqual)
	{
		TypeParam v1 = Value<TypeParam>(0);
		TypeParam v2 = Value<TypeParam>(1);
		TypeParam v3 = Value<TypeParam>(1);
		EXPECT_TRUE(Algorithm::NotEqual(v1, v2));
		EXPECT_TRUE(Algorithm::NotEqual(v1, v3));
		EXPECT_FALSE(Algorithm::NotEqual(v2, v3));
		EXPECT_TRUE(Algorithm::NotEqual(v2, v1));
		EXPECT_TRUE(Algorithm::NotEqual(v3, v1));
		EXPECT_FALSE(Algorithm::NotEqual(v3, v2));
	}

	TYPED_TEST_P(CompareTest, Compare_Less_EqualSize)
	{
		static constexpr Array k_lhs = { Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) };
		static constexpr Array k_rhs = { Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(3) };
		Int32 result                 = Compare(k_lhs.Data(), k_rhs.Data(), k_lhs.Size(), k_rhs.Size());
		ASSERT_EQ(result, -1);
	}

	TYPED_TEST_P(CompareTest, Compare_Equal_EqualSize)
	{
		static constexpr Array k_lhs = { Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) };
		static constexpr Array k_rhs = { Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) };
		Int32 result                 = Compare(k_lhs.Data(), k_rhs.Data(), k_lhs.Size(), k_rhs.Size());
		ASSERT_EQ(result, 0);
	}

	TYPED_TEST_P(CompareTest, Compare_Greater_EqualSize)
	{
		static constexpr Array k_lhs = { Value<TypeParam>(0), Value<TypeParam>(2), Value<TypeParam>(2) };
		static constexpr Array k_rhs = { Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) };
		Int32 result                 = Compare(k_lhs.Data(), k_rhs.Data(), k_lhs.Size(), k_rhs.Size());
		ASSERT_EQ(result, 1);
	}

	TYPED_TEST_P(CompareTest, Compare_Less_DifferentSize)
	{
		static constexpr Array k_lhs = { Value<TypeParam>(0), Value<TypeParam>(1) };
		static constexpr Array k_rhs = { Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(3) };
		Int32 result                 = Compare(k_lhs.Data(), k_rhs.Data(), k_lhs.Size(), k_rhs.Size());
		ASSERT_EQ(result, -1);
	}
	TYPED_TEST_P(CompareTest, Compare_Greater_DifferentSize)
	{
		static constexpr Array k_lhs = { Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) };
		static constexpr Array k_rhs = { Value<TypeParam>(0), Value<TypeParam>(1) };
		Int32 result                 = Compare(k_lhs.Data(), k_rhs.Data(), k_lhs.Size(), k_rhs.Size());
		ASSERT_EQ(result, 1);
	}

	REGISTER_TYPED_TEST_SUITE_P(CompareTest,
	                            Less,
	                            LessEqual,
	                            Greater,
	                            GreaterEqual,
	                            Equal,
	                            NotEqual,
	                            Compare_Less_EqualSize,
	                            Compare_Equal_EqualSize,
	                            Compare_Greater_EqualSize,
	                            Compare_Less_DifferentSize,
	                            Compare_Greater_DifferentSize);

	INSTANTIATE_TYPED_TEST_SUITE_P(ComparePrimitiveTypesTest, CompareTest, PrimitiveTypes);
	INSTANTIATE_TYPED_TEST_SUITE_P(CompareComplexTypesTest, CompareTest, ComplexType);

}  // namespace Orion::Engine::Algorithm::UT
