#include "TestCommon.h"

#include "Core/Standard/Memory/Allocators/DummyAllocator.h"
#include "Core/Standard/Containers/Array.h"
#include "Core/Standard/Containers/HashMap.h"

namespace Orion::Engine::UT
{
	namespace
	{
		template <typename T>
		class HashMapTest : public ::testing::Test
		{
		};
	}  // namespace
	TYPED_TEST_SUITE_P(HashMapTest);

	template <typename KeyT, typename ValueT = KeyT>
	static constexpr HashMap<KeyT, ValueT>::KeyValueType Value(USize index)
	{
		return MakePair(static_cast<KeyT>(index), static_cast<ValueT>(index + 1));
	}

	template <>
	constexpr HashMap<ComplexType, ComplexType>::KeyValueType Value<ComplexType>(USize index)
	{
		return MakePair(ComplexType(static_cast<ComplexType::ValueType>(index)),
		                ComplexType(static_cast<ComplexType::ValueType>(index + 1)));
	}

	TYPED_TEST_P(HashMapTest, Constructor_InitialCapacity)
	{
		HashMap h = HashMap<TypeParam,
		                    TypeParam,
		                    Algorithm::Hash<TypeParam>,
		                    Algorithm::Equal<TypeParam>,
		                    Memory::DummyAllocator>(8, Memory::DummyAllocator());
		EXPECT_TRUE(h.IsEmpty());
		EXPECT_EQ(h.Size(), 0);
		EXPECT_EQ(h.ByteSize(), 0);
		EXPECT_EQ(h.Capacity(), decltype(h)::k_initial_bucket_count);
	}

	TYPED_TEST_P(HashMapTest, Constructor_InitializerList_Empty)
	{
		HashMap h = HashMap<TypeParam,
		                    TypeParam,
		                    Algorithm::Hash<TypeParam>,
		                    Algorithm::Equal<TypeParam>,
		                    Memory::DummyAllocator>({});
		EXPECT_TRUE(h.IsEmpty());
		EXPECT_EQ(h.Size(), 0);
		EXPECT_EQ(h.ByteSize(), 0);
		EXPECT_EQ(h.Capacity(), decltype(h)::k_initial_bucket_count);
	}

	TYPED_TEST_P(HashMapTest, Constructor_InitializerList)
	{
		HashMap h = HashMap<TypeParam, TypeParam>({
			Value<TypeParam>(0),
			Value<TypeParam>(1),
			Value<TypeParam>(2),
			Value<TypeParam>(3),
			Value<TypeParam>(4),
		});
		EXPECT_FALSE(h.IsEmpty());
		ASSERT_EQ(h.Size(), 5);
		EXPECT_EQ(h.ByteSize(), h.Size() * sizeof(typename decltype(h)::KeyValueType));
		EXPECT_EQ(h.Capacity(), decltype(h)::k_initial_bucket_count);

		for (USize index = 0; index < h.Size(); ++index) {
			TypeParam expected_key   = static_cast<TypeParam>(index);
			TypeParam expected_value = static_cast<TypeParam>(index + 1);
			ASSERT_TRUE(h.Contains(expected_key));

			TypeParam* result_value = h.Find(expected_key);
			ASSERT_TRUE(result_value);
			EXPECT_EQ(expected_value, *result_value);
		}
	}

	TYPED_TEST_P(HashMapTest, Constructor_Copy)
	{
		HashMap h1 = HashMap<TypeParam, TypeParam>({ Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) });
		HashMap h2(h1);

		EXPECT_FALSE(h1.IsEmpty());
		EXPECT_FALSE(h2.IsEmpty());
		EXPECT_EQ(h1.Size(), h2.Size());
		EXPECT_EQ(h1.ByteSize(), h2.ByteSize());
		EXPECT_EQ(h1.Capacity(), h2.Capacity());

		for (USize index = 0; index < h1.Size(); ++index) {
			TypeParam expected_key   = static_cast<TypeParam>(index);
			TypeParam expected_value = static_cast<TypeParam>(index + 1);
			ASSERT_TRUE(h1.Contains(expected_key));
			ASSERT_TRUE(h2.Contains(expected_key));

			TypeParam* result_value_h1 = h1.Find(expected_key);
			TypeParam* result_value_h2 = h2.Find(expected_key);
			ASSERT_TRUE(result_value_h1);
			ASSERT_TRUE(result_value_h2);
			EXPECT_EQ(*result_value_h1, expected_value);
			EXPECT_EQ(*result_value_h1, *result_value_h2);
		}
	}

	TYPED_TEST_P(HashMapTest, Constructor_Move)
	{
		HashMap h1 = HashMap<TypeParam, TypeParam>({ Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) });
		HashMap h2(Move(h1));

		EXPECT_EQ(h2.Size(), 3);
		EXPECT_EQ(h2.ByteSize(), h2.Size() * sizeof(typename decltype(h2)::KeyValueType));
		EXPECT_EQ(h2.Capacity(), decltype(h2)::k_initial_bucket_count);
		for (USize index = 0; index < h2.Size(); ++index) {
			TypeParam expected_key   = static_cast<TypeParam>(index);
			TypeParam expected_value = static_cast<TypeParam>(index + 1);
			ASSERT_TRUE(h2.Contains(expected_key));

			TypeParam* result_value = h2.Find(expected_key);
			ASSERT_TRUE(result_value);
			EXPECT_EQ(*result_value, expected_value);
		}
	}

	TYPED_TEST_P(HashMapTest, Operator_Copy)
	{
		HashMap h1 = HashMap<TypeParam, TypeParam>({ Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) });
		EXPECT_FALSE(h1.IsEmpty());
		EXPECT_EQ(h1.Size(), 3);
		EXPECT_EQ(h1.ByteSize(), h1.Size() * sizeof(typename decltype(h1)::KeyValueType));
		EXPECT_EQ(h1.Capacity(), decltype(h1)::k_initial_bucket_count);

		HashMap h2 = HashMap<TypeParam, TypeParam>({});
		EXPECT_TRUE(h2.IsEmpty());
		EXPECT_EQ(h2.Size(), 0);
		EXPECT_EQ(h2.ByteSize(), 0);
		EXPECT_EQ(h2.Capacity(), decltype(h2)::k_initial_bucket_count);

		h2 = h1;
		EXPECT_FALSE(h1.IsEmpty());
		EXPECT_FALSE(h2.IsEmpty());
		EXPECT_EQ(h1.Size(), h2.Size());
		EXPECT_EQ(h1.ByteSize(), h2.ByteSize());
		EXPECT_EQ(h1.Capacity(), h2.Capacity());

		for (USize index = 0; index < h1.Size(); ++index) {
			TypeParam expected_key   = static_cast<TypeParam>(index);
			TypeParam expected_value = static_cast<TypeParam>(index + 1);
			ASSERT_TRUE(h1.Contains(expected_key));
			ASSERT_TRUE(h2.Contains(expected_key));

			TypeParam* result_value_h1 = h1.Find(expected_key);
			TypeParam* result_value_h2 = h2.Find(expected_key);
			ASSERT_TRUE(result_value_h1);
			ASSERT_TRUE(result_value_h2);
			EXPECT_EQ(*result_value_h1, expected_value);
			EXPECT_EQ(*result_value_h1, *result_value_h2);
		}
	}

	TYPED_TEST_P(HashMapTest, Operator_Move)
	{
		HashMap h1 = HashMap<TypeParam, TypeParam>({ Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) });
		EXPECT_FALSE(h1.IsEmpty());
		EXPECT_EQ(h1.Size(), 3);
		EXPECT_EQ(h1.ByteSize(), h1.Size() * sizeof(typename decltype(h1)::KeyValueType));
		EXPECT_EQ(h1.Capacity(), decltype(h1)::k_initial_bucket_count);

		HashMap h2 = HashMap<TypeParam, TypeParam>({});
		EXPECT_TRUE(h2.IsEmpty());
		EXPECT_EQ(h2.Size(), 0);
		EXPECT_EQ(h2.ByteSize(), 0);
		EXPECT_EQ(h2.Capacity(), decltype(h2)::k_initial_bucket_count);

		h2 = Move(h1);
		EXPECT_EQ(h2.Size(), 3);
		EXPECT_EQ(h2.ByteSize(), h2.Size() * sizeof(typename decltype(h2)::KeyValueType));
		EXPECT_EQ(h2.Capacity(), decltype(h2)::k_initial_bucket_count);
		for (USize index = 0; index < h2.Size(); ++index) {
			TypeParam expected_key   = static_cast<TypeParam>(index);
			TypeParam expected_value = static_cast<TypeParam>(index + 1);
			ASSERT_TRUE(h2.Contains(expected_key));

			TypeParam* result_value = h2.Find(expected_key);
			ASSERT_TRUE(result_value);
			EXPECT_EQ(*result_value, expected_value);
		}
	}

	REGISTER_TYPED_TEST_SUITE_P(HashMapTest,
	                            Constructor_InitialCapacity,
	                            Constructor_InitializerList_Empty,
	                            Constructor_InitializerList,
	                            Constructor_Copy,
	                            Constructor_Move,
	                            Operator_Copy,
	                            Operator_Move);

	INSTANTIATE_TYPED_TEST_SUITE_P(HashMapPrimitiveTypesTest, HashMapTest, PrimitiveTypes);
	INSTANTIATE_TYPED_TEST_SUITE_P(HashMapComplexTypesTest, HashMapTest, ComplexType);
}  // namespace Orion::Engine::UT
