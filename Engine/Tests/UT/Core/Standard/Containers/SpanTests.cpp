#include "TestCommon.h"

#include "Core/Standard/Containers/Array.h"
#include "Core/Standard/Containers/Span.h"

namespace Orion::Engine::UT
{
	class SpanTest : public ::testing::Test
	{
	};

	TEST_F(SpanTest, Comparison_EmptySpan)
	{
		static constexpr Span<UInt32> k_first_span  = {};
		static constexpr Span<UInt32> k_second_span = {};
		ASSERT_EQ(k_first_span, k_second_span);
	}

	TEST_F(SpanTest, Comparison_NonEmptySpan_Equal)
	{
		static constexpr Array k_array      = { 0, 1, 2, 3, 4, 5 };
		static constexpr Span k_first_span  = Span(k_array);
		static constexpr Span k_second_span = Span(k_array);
		ASSERT_EQ(k_first_span, k_second_span);
	}

	TEST_F(SpanTest, Comparison_NonEmptySpan_NotEqual)
	{
		static constexpr Array k_array      = { 0, 1, 2, 3, 4, 5 };
		static constexpr Span k_first_span  = Span(k_array.Data() + 1, 3);
		static constexpr Span k_second_span = Span(k_array.Data(), k_array.Size());
		ASSERT_NE(k_first_span, k_second_span);
	}

	TEST_F(SpanTest, Capacity_EmptySpan)
	{
		static constexpr Span k_span = Span<UInt32>();
		EXPECT_TRUE(k_span.IsEmpty());
		EXPECT_EQ(k_span.Size(), 0);
	}

	TEST_F(SpanTest, Capacity_NonEmptySpan)
	{
		static constexpr Array k_array = { 0, 1, 2, 3, 4, 5 };
		static constexpr Span k_span   = Span(k_array);
		EXPECT_FALSE(k_span.IsEmpty());
		EXPECT_EQ(k_span.Size(), k_array.Size());
	}

	TEST_F(SpanTest, ElementAccess_Read)
	{
		static constexpr Array k_array      = { 0, 1, 2, 3, 4, 5 };
		static constexpr Span k_result_span = Span(k_array);

		ASSERT_EQ(k_array.Size(), k_result_span.Size());
		for (USize index = 0; index < k_result_span.Size(); ++index) {
			EXPECT_EQ(k_array[index], k_result_span[index]);
			EXPECT_EQ(*(k_array.Data() + index), *(k_result_span.Data() + index));
		}
		EXPECT_EQ(k_array.Back(), k_result_span.Back());
		EXPECT_EQ(k_array.Front(), k_result_span.Front());
	}

	TEST_F(SpanTest, ElementAccess_Write)
	{
		Array result_array = { 0, 1, 2, 3, 4, 5 };
		Span result_span   = Span(result_array);

		// Values should not change before any modification takes place.
		ASSERT_EQ(result_array.Size(), result_array.Size());
		for (USize index = 0; index < result_array.Size(); ++index) {
			EXPECT_EQ(result_array[index], index);
		}

		for (USize index = 0; index < result_span.Size(); ++index) {
			result_span[index] += 5;
		}

		static constexpr Array k_expected_array = { 5, 6, 7, 8, 9, 10 };
		ASSERT_EQ(result_array.Size(), k_expected_array.Size());
		ASSERT_EQ(result_array, k_expected_array);
	}

	TEST_F(SpanTest, ElementAccess_Iterators)
	{
		static constexpr Array k_array = { 0, 1, 2, 3, 4, 5 };
		static constexpr Span k_span   = Span(k_array);

		ASSERT_EQ(k_array.Size(), k_span.Size());
		auto it_array = k_array.begin();
		auto it_span  = k_span.begin();
		for (USize index = 0; index < k_span.Size(); ++index) {
			EXPECT_EQ(*it_array++, *it_span++);
		}
	}
}  // namespace Orion::Engine::UT
