#include "TestCommon.h"

#include "OrionEngine.h"

#include "Core/Standard/Containers/Array.h"

namespace Orion::Engine::UT
{
	template <typename T>
	class ArrayTest : public ::testing::Test
	{
	};

	template <typename T>
	static constexpr T GetArrayValue(USize index)
	{
		return static_cast<T>(index);
	}
	TYPED_TEST_SUITE(ArrayTest, PrimitiveTypes);

	TYPED_TEST(ArrayTest, Comparison_EmptyArray)
	{
		static constexpr Array<TypeParam, 0> k_first_empty_array  = {};
		static constexpr Array<TypeParam, 0> k_second_empty_array = {};
		ASSERT_EQ(k_first_empty_array, k_second_empty_array);
	}

	TYPED_TEST(ArrayTest, Comparison_NonEmptyArray)
	{
		static constexpr Array<TypeParam, 3> k_first_array = {
			GetArrayValue<TypeParam>(0),
			GetArrayValue<TypeParam>(1),
			GetArrayValue<TypeParam>(2),
		};
		static constexpr Array<TypeParam, 3> k_second_array = {
			GetArrayValue<TypeParam>(0),
			GetArrayValue<TypeParam>(8),
			GetArrayValue<TypeParam>(-9),
		};
		static constexpr Array<TypeParam, 3> k_third_array = {
			GetArrayValue<TypeParam>(0),
			GetArrayValue<TypeParam>(1),
			GetArrayValue<TypeParam>(2),
		};

		EXPECT_NE(k_first_array, k_second_array);
		EXPECT_EQ(k_first_array, k_third_array);
		EXPECT_NE(k_second_array, k_third_array);
	}

	TYPED_TEST(ArrayTest, Capacity_EmptyArray)
	{
		static constexpr Array<TypeParam, 0> k_array = {};
		EXPECT_TRUE(k_array.IsEmpty());
		EXPECT_EQ(k_array.Size(), 0);
		EXPECT_EQ(k_array.ByteSize(), sizeof(TypeParam) * k_array.Size());
	}

	TYPED_TEST(ArrayTest, Capacity_NonEmptyArray)
	{
		static constexpr Array<TypeParam, 3> k_array = {
			GetArrayValue<TypeParam>(0),
			GetArrayValue<TypeParam>(1),
			GetArrayValue<TypeParam>(2),
		};

		EXPECT_FALSE(k_array.IsEmpty());
		ASSERT_EQ(k_array.Size(), 3);
		EXPECT_EQ(k_array.ByteSize(), sizeof(TypeParam) * k_array.Size());
	}

	TYPED_TEST(ArrayTest, ElementAccess_Read)
	{
		static constexpr Array<TypeParam, 3> k_array = {
			GetArrayValue<TypeParam>(0),
			GetArrayValue<TypeParam>(1),
			GetArrayValue<TypeParam>(2),
		};
		using SizeType = decltype(k_array)::SizeType;

		ASSERT_EQ(k_array.Size(), 3);
		for (SizeType index = 0; index < k_array.Size(); ++index) {
			EXPECT_EQ(k_array[index], GetArrayValue<TypeParam>(index));
			EXPECT_EQ(*(k_array.Data() + index), GetArrayValue<TypeParam>(index));
		}
		EXPECT_EQ(k_array.Front(), GetArrayValue<TypeParam>(0));
		EXPECT_EQ(k_array.Back(), GetArrayValue<TypeParam>(2));
	}

	TYPED_TEST(ArrayTest, ElementAccess_Write)
	{
		Array<TypeParam, 3> k_array = {
			GetArrayValue<TypeParam>(0),
			GetArrayValue<TypeParam>(1),
			GetArrayValue<TypeParam>(2),
		};
		using SizeType = decltype(k_array)::SizeType;

		// Values should not change before any modification takes place.
		ASSERT_EQ(k_array.Size(), 3);
		for (SizeType index = 0; index < k_array.Size(); ++index) {
			EXPECT_EQ(k_array[index], GetArrayValue<TypeParam>(index));
			EXPECT_EQ(*(k_array.Data() + index), GetArrayValue<TypeParam>(index));
		}
		EXPECT_EQ(k_array.Front(), GetArrayValue<TypeParam>(0));
		EXPECT_EQ(k_array.Back(), GetArrayValue<TypeParam>(2));

		for (SizeType index = 0; index < k_array.Size(); ++index) {
			k_array[index] = index * 101;
		}

		// Values should now be equal
		static constexpr Array k_expected_array = {
			GetArrayValue<TypeParam>(0),
			GetArrayValue<TypeParam>(101),
			GetArrayValue<TypeParam>(202),
		};
		ASSERT_EQ(k_array.Size(), k_expected_array.Size());
		for (SizeType index = 0; index < k_array.Size(); ++index) {
			EXPECT_EQ(k_array[index], k_expected_array[index]);
			EXPECT_EQ(*(k_array.Data() + index), *(k_expected_array.Data() + index));
		}
		EXPECT_EQ(k_array.Front(), k_expected_array.Front());
		EXPECT_EQ(k_array.Back(), k_expected_array.Back());
	}

	TYPED_TEST(ArrayTest, ElementAccess_Iterators)
	{
		static constexpr Array<TypeParam, 3> k_array = {
			GetArrayValue<TypeParam>(0),
			GetArrayValue<TypeParam>(1),
			GetArrayValue<TypeParam>(2),
		};
		using SizeType         = decltype(k_array)::SizeType;
		using ConstPointerType = decltype(k_array)::ConstPointerType;

		SizeType index = 0;
		ASSERT_EQ(k_array.Size(), 3);
		for (ConstPointerType it = k_array.begin(); it != k_array.end(); ++it) {
			EXPECT_EQ(*it, GetArrayValue<TypeParam>(index++));
		}
	}

	TYPED_TEST(ArrayTest, Fill)
	{
		Array<TypeParam, 3> k_array = {
			GetArrayValue<TypeParam>(0),
			GetArrayValue<TypeParam>(1),
			GetArrayValue<TypeParam>(2),
		};
		using SizeType = decltype(k_array)::SizeType;

		ASSERT_EQ(k_array.Size(), 3);
		for (SizeType index = 0; index < k_array.Size(); ++index) {
			EXPECT_EQ(k_array[index], GetArrayValue<TypeParam>(index));
		}

		k_array.Fill(GetArrayValue<TypeParam>(123));

		ASSERT_EQ(k_array.Size(), 3);
		for (SizeType index = 0; index < k_array.Size(); ++index) {
			EXPECT_EQ(k_array[index], GetArrayValue<TypeParam>(123));
		}
	}

	TYPED_TEST(ArrayTest, Sort)
	{
		Array<TypeParam, 3> k_array = {
			GetArrayValue<TypeParam>(5),
			GetArrayValue<TypeParam>(8),
			GetArrayValue<TypeParam>(1),
		};
		k_array.Sort([](const TypeParam& lhs, const TypeParam& rhs) -> bool { return lhs < rhs; });
		static constexpr Array k_expected_array = {
			GetArrayValue<TypeParam>(1),
			GetArrayValue<TypeParam>(5),
			GetArrayValue<TypeParam>(8),
		};
		ASSERT_EQ(k_array, k_expected_array);
	}
}  // namespace Orion::Engine::UT
