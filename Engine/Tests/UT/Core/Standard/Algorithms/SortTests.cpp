#include "TestCommon.h"

#include "Core/Standard/Algorithms/Sort.h"
#include "Core/Standard/Containers/Array.h"

namespace Orion::Engine::UT
{
	namespace
	{
		template <typename T>
		class SortTest : public ::testing::Test
		{
		};
	}  // namespace
	TYPED_TEST_SUITE_P(SortTest);

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

	TYPED_TEST_P(SortTest, Quicksort)
	{
		using T = TypeParam;
		{
			Array arr = { Value<T>(9), Value<T>(0), Value<T>(2), Value<T>(5), Value<T>(3) };
			Algorithm::Quicksort(arr.Data(), arr.Size(), Algorithm::Less<typename decltype(arr)::ValueType>);
			static constexpr Array k_expected_array
				= { Value<T>(0), Value<T>(2), Value<T>(3), Value<T>(5), Value<T>(9) };
			EXPECT_EQ(arr, k_expected_array);
		}

		{
			Array arr = { Value<T>(3), Value<T>(9), Value<T>(0), Value<T>(2), Value<T>(3) };
			Algorithm::Quicksort(arr.Data(), arr.Size(), Algorithm::LessEqual<typename decltype(arr)::ValueType>);
			static constexpr Array k_expected_array
				= { Value<T>(0), Value<T>(2), Value<T>(3), Value<T>(3), Value<T>(9) };
			EXPECT_EQ(arr, k_expected_array);
		}

		{
			Array arr = { Value<T>(9), Value<T>(0), Value<T>(2), Value<T>(5), Value<T>(3) };
			Algorithm::Quicksort(arr.Data(), arr.Size(), Algorithm::Greater<typename decltype(arr)::ValueType>);
			static constexpr Array k_expected_array
				= { Value<T>(9), Value<T>(5), Value<T>(3), Value<T>(2), Value<T>(0) };
			EXPECT_EQ(arr, k_expected_array);
		}

		{
			Array arr = { Value<T>(3), Value<T>(9), Value<T>(0), Value<T>(2), Value<T>(3) };
			Algorithm::Quicksort(arr.Data(), arr.Size(), Algorithm::GreaterEqual<typename decltype(arr)::ValueType>);
			static constexpr Array k_expected_array
				= { Value<T>(9), Value<T>(3), Value<T>(3), Value<T>(2), Value<T>(0) };
			EXPECT_EQ(arr, k_expected_array);
		}

		{
			Array arr = { Value<T>(3), Value<T>(9), Value<T>(3), Value<T>(0), Value<T>(9) };
			Algorithm::Quicksort(arr.Data(), arr.Size(), Algorithm::Equal<typename decltype(arr)::ValueType>);
			static constexpr Array k_expected_array
				= { Value<T>(9), Value<T>(9), Value<T>(3), Value<T>(3), Value<T>(0) };
			EXPECT_EQ(arr, k_expected_array);
		}

		{
			Array arr = { Value<T>(3), Value<T>(9), Value<T>(3), Value<T>(0), Value<T>(9) };
			Algorithm::Quicksort(arr.Data(), arr.Size(), Algorithm::NotEqual<typename decltype(arr)::ValueType>);
			static constexpr Array k_expected_array
				= { Value<T>(3), Value<T>(3), Value<T>(0), Value<T>(9), Value<T>(9) };
			EXPECT_EQ(arr, k_expected_array);
		}
	}

	REGISTER_TYPED_TEST_SUITE_P(SortTest, Quicksort);

	INSTANTIATE_TYPED_TEST_SUITE_P(SortPrimitiveTypesTest, SortTest, PrimitiveTypes);
	INSTANTIATE_TYPED_TEST_SUITE_P(SortComplexTypesTest, SortTest, ComplexType);
}  // namespace Orion::Engine::UT
