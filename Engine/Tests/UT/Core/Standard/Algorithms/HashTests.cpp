#include "TestCommon.h"

#include "Core/Standard/Algorithms/Hash.h"
#include "Core/Standard/Containers/Array.h"

namespace Orion::Engine::Algorithm::UT
{
	using namespace Orion::Engine::UT;

	namespace
	{
		template <typename T>
		class HashTest : public ::testing::Test
		{
		};
	}  // namespace
	TYPED_TEST_SUITE_P(HashTest);

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

	TYPED_TEST_P(HashTest, HashT)
	{
		TypeParam v1 = Value<TypeParam>(0);
		TypeParam v2 = Value<TypeParam>(1);
		TypeParam v3 = Value<TypeParam>(1);

		typename Hash<TypeParam>::SizeType h1 = Hash<TypeParam>()(v1);
		typename Hash<TypeParam>::SizeType h2 = Hash<TypeParam>()(v2);
		typename Hash<TypeParam>::SizeType h3 = Hash<TypeParam>()(v3);

		ASSERT_NE(h1, h2);
		ASSERT_NE(h1, h3);
		ASSERT_EQ(h2, h3);
	}

	REGISTER_TYPED_TEST_SUITE_P(HashTest, HashT);

	INSTANTIATE_TYPED_TEST_SUITE_P(HashPrimitiveTypesTest, HashTest, PrimitiveTypes);
	INSTANTIATE_TYPED_TEST_SUITE_P(HashComplexTypesTest, HashTest, ComplexType);

}  // namespace Orion::Engine::Algorithm::UT
