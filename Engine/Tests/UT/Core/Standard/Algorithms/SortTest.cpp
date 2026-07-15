#include "TestCommon.h"

#include "Core/Standard/Containers/Array.h"

namespace Orion::Engine::UT
{
	class SortTest : public ::testing::Test
	{
	};

	template <typename T>
	static constexpr T Value(USize index)
	{
		return static_cast<T>(index);
	}

	TEST_F(SortTest, Quicksort)
	{
		{
			Array arr = { 9, 0, 2, 5, 3 };
			Algorithm::Quicksort(arr.Data(), arr.Size(), Algorithm::Compare::Less<decltype(arr)::ValueType>);
			static constexpr Array k_expected_array = { 0, 2, 3, 5, 9 };
			EXPECT_EQ(arr, k_expected_array);
		}

		{
			Array arr = { 3, 9, 0, 2, 3 };
			Algorithm::Quicksort(arr.Data(), arr.Size(), Algorithm::Compare::LessEqual<decltype(arr)::ValueType>);
			static constexpr Array k_expected_array = { 0, 2, 3, 3, 9 };
			EXPECT_EQ(arr, k_expected_array);
		}

		{
			Array arr = { 9, 0, 2, 5, 3 };
			Algorithm::Quicksort(arr.Data(), arr.Size(), Algorithm::Compare::Greater<decltype(arr)::ValueType>);
			static constexpr Array k_expected_array = { 9, 5, 3, 2, 0 };
			EXPECT_EQ(arr, k_expected_array);
		}

		{
			Array arr = { 3, 9, 0, 2, 3 };
			Algorithm::Quicksort(arr.Data(), arr.Size(), Algorithm::Compare::GreaterEqual<decltype(arr)::ValueType>);
			static constexpr Array k_expected_array = { 9, 3, 3, 2, 0 };
			EXPECT_EQ(arr, k_expected_array);
		}

		{
			Array arr = { 3, 9, 3, 0, 9 };
			Algorithm::Quicksort(arr.Data(), arr.Size(), Algorithm::Compare::Equal<decltype(arr)::ValueType>);
			static constexpr Array k_expected_array = { 9, 9, 3, 3, 0 };
			EXPECT_EQ(arr, k_expected_array);
		}

		{
			Array arr = { 3, 9, 3, 0, 9 };
			Algorithm::Quicksort(arr.Data(), arr.Size(), Algorithm::Compare::NotEqual<decltype(arr)::ValueType>);
			static constexpr Array k_expected_array = { 3, 3, 0, 9, 9 };
			EXPECT_EQ(arr, k_expected_array);
		}
	}
}  // namespace Orion::Engine::UT
