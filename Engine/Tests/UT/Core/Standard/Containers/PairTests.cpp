#include "TestCommon.h"

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

	TYPED_TEST_P(PairTest, Foo)
	{
		GTEST_FAIL();
	}

	REGISTER_TYPED_TEST_SUITE_P(PairTest, Foo);

	INSTANTIATE_TYPED_TEST_SUITE_P(PairPrimitiveTypesTest, PairTest, PrimitiveTypes);
	INSTANTIATE_TYPED_TEST_SUITE_P(PairComplexTypesTest, PairTest, ComplexType);
}  // namespace Orion::Engine::UT
