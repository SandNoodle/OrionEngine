#include "TestCommon.h"

#include "Core/Standard/Containers/Pair.h"

namespace Orion::Engine::UT
{
	namespace
	{
		template <typename T>
		class PairTest : public ::testing::Test
		{
		};
	}  // namespace
	TYPED_TEST_SUITE_P(PairTest);

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

	TYPED_TEST_P(PairTest, Util_MakePair)
	{
		Pair p = MakePair<TypeParam, TypeParam>(Value<TypeParam>(1), Value<TypeParam>(2));
		EXPECT_EQ(p.first, Value<TypeParam>(1));
		EXPECT_EQ(p.second, Value<TypeParam>(2));
	}

	REGISTER_TYPED_TEST_SUITE_P(PairTest, Util_MakePair);

	INSTANTIATE_TYPED_TEST_SUITE_P(PairPrimitiveTypesTest, PairTest, PrimitiveTypes);
	INSTANTIATE_TYPED_TEST_SUITE_P(PairComplexTypesTest, PairTest, ComplexType);
}  // namespace Orion::Engine::UT
