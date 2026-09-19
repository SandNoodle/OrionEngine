#include "TestCommon.h"

#include "Core/Standard/Containers/HashMap.h"
#include "Core/Standard/Containers/Pair.h"
#include "Core/Standard/Memory/Allocators/DummyAllocator.h"

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
		return Detail::HashMapElement(static_cast<KeyT>(index), static_cast<ValueT>(index + 1));
	}

	template <>
	constexpr HashMap<ComplexType, ComplexType>::KeyValueType Value<ComplexType>(USize index)
	{
		return Detail::HashMapElement(ComplexType(static_cast<ComplexType::ValueType>(index)),
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
		HashMap h1({ Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) });
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

	TYPED_TEST_P(HashMapTest, Operator_Subscript)
	{
		HashMap h{ Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) };
		EXPECT_FALSE(h.IsEmpty());
		EXPECT_EQ(h.Size(), 3);
		EXPECT_EQ(h.ByteSize(), h.Size() * sizeof(typename decltype(h)::KeyValueType));
		EXPECT_EQ(h.Capacity(), decltype(h)::k_initial_bucket_count);

		for (USize index = 0; index < h.Size(); ++index) {
			TypeParam key   = TypeParam(index);
			TypeParam value = TypeParam(index + 1);
			EXPECT_TRUE(h.Contains(key));
			EXPECT_EQ(h[key], value);
		}
	}

	TYPED_TEST_P(HashMapTest, Iterator_BeginEnd_NonConst)
	{
		HashMap h{ Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) };
		EXPECT_FALSE(h.IsEmpty());
		EXPECT_EQ(h.Size(), 3);
		EXPECT_EQ(h.ByteSize(), h.Size() * sizeof(typename decltype(h)::KeyValueType));
		EXPECT_EQ(h.Capacity(), decltype(h)::k_initial_bucket_count);

		using IteratorType            = decltype(h)::IteratorType;
		IteratorType begin            = h.begin();
		IteratorType end              = h.end();
		USize processed_element_count = 0UL;
		for (IteratorType it = begin; it != h.end(); ++it) {
			typename IteratorType::ReferenceType e = *it;
			EXPECT_TRUE(h.Contains(it->key));
			++processed_element_count;
		}
		EXPECT_EQ(processed_element_count, h.Size());
	}

	TYPED_TEST_P(HashMapTest, Iterator_BeginEnd_Const)
	{
		const HashMap h{ Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) };
		EXPECT_FALSE(h.IsEmpty());
		EXPECT_EQ(h.Size(), 3);
		EXPECT_EQ(h.ByteSize(), h.Size() * sizeof(typename decltype(h)::KeyValueType));
		EXPECT_EQ(h.Capacity(), decltype(h)::k_initial_bucket_count);

		using ConstIteratorType       = decltype(h)::ConstIteratorType;
		ConstIteratorType begin       = h.begin();
		ConstIteratorType end         = h.end();
		USize processed_element_count = 0UL;
		for (ConstIteratorType it = begin; it != h.end(); ++it) {
			typename ConstIteratorType::ReferenceType e = *it;
			EXPECT_TRUE(h.Contains(it->key));
			++processed_element_count;
		}
		EXPECT_EQ(processed_element_count, h.Size());
	}

	TYPED_TEST_P(HashMapTest, Insert)
	{
		HashMap<TypeParam, TypeParam> h{};
		EXPECT_TRUE(h.IsEmpty());
		EXPECT_EQ(h.Size(), 0UL);
		EXPECT_EQ(h.ByteSize(), h.Size() * sizeof(typename decltype(h)::KeyValueType));
		EXPECT_EQ(h.Capacity(), decltype(h)::k_initial_bucket_count);

		// Insert(const KeyType&, const ValueType&)
		{
			const TypeParam key   = TypeParam(0UL);
			const TypeParam value = TypeParam(1UL);
			h.Insert(key, value);
			EXPECT_EQ(h.Size(), 1UL);
			EXPECT_EQ(h.ByteSize(), h.Size() * sizeof(typename decltype(h)::KeyValueType));
			EXPECT_EQ(h.Capacity(), decltype(h)::k_initial_bucket_count);
			ASSERT_TRUE(h.Contains(key));
			EXPECT_EQ(h[key], TypeParam(1UL));
		}

		// Insert(const KeyType&, ValueType&&)
		{
			const TypeParam key = TypeParam(1UL);
			TypeParam value     = TypeParam(2UL);
			h.Insert(key, Move(value));
			EXPECT_EQ(h.Size(), 2UL);
			EXPECT_EQ(h.ByteSize(), h.Size() * sizeof(typename decltype(h)::KeyValueType));
			EXPECT_EQ(h.Capacity(), decltype(h)::k_initial_bucket_count);
			ASSERT_TRUE(h.Contains(key));
			EXPECT_EQ(h[key], TypeParam(2UL));
		}

		// Insert(KeyType&&, const ValueType&)
		{
			TypeParam key         = TypeParam(2UL);
			const TypeParam value = TypeParam(3UL);
			h.Insert(Move(key), value);
			EXPECT_EQ(h.Size(), 3UL);
			EXPECT_EQ(h.ByteSize(), h.Size() * sizeof(typename decltype(h)::KeyValueType));
			EXPECT_EQ(h.Capacity(), decltype(h)::k_initial_bucket_count);
			ASSERT_TRUE(h.Contains(TypeParam(2UL)));
			EXPECT_EQ(h[TypeParam(2UL)], TypeParam(3UL));
		}

		// Insert(KeyType&&, ValueType&&)
		{
			TypeParam key         = TypeParam(3UL);
			const TypeParam value = TypeParam(4UL);
			h.Insert(Move(key), value);
			EXPECT_EQ(h.Size(), 4UL);
			EXPECT_EQ(h.ByteSize(), h.Size() * sizeof(typename decltype(h)::KeyValueType));
			EXPECT_EQ(h.Capacity(), decltype(h)::k_initial_bucket_count);
			ASSERT_TRUE(h.Contains(TypeParam(3UL)));
			EXPECT_EQ(h[TypeParam(3UL)], TypeParam(4UL));
		}
	}

	TYPED_TEST_P(HashMapTest, Insert_ManyValuesCausingRehash)
	{
		HashMap<TypeParam, TypeParam> h{};
		EXPECT_TRUE(h.IsEmpty());
		EXPECT_EQ(h.Size(), 0UL);
		EXPECT_EQ(h.ByteSize(), h.Size() * sizeof(typename decltype(h)::KeyValueType));
		EXPECT_EQ(h.Capacity(), decltype(h)::k_initial_bucket_count);

		static constexpr USize k_target_element_count = decltype(h)::k_initial_bucket_count * 2UL - 1UL;
		for (USize index = 0UL; index < k_target_element_count; ++index) {
			h.Insert(TypeParam(index), TypeParam(index + 1UL));
		}

		EXPECT_FALSE(h.IsEmpty());
		EXPECT_EQ(h.Size(), k_target_element_count);
		EXPECT_EQ(h.ByteSize(), h.Size() * sizeof(typename decltype(h)::KeyValueType));
		EXPECT_GE(h.Capacity(), k_target_element_count);
		for (USize index = 0UL; index < k_target_element_count; ++index) {
			const TypeParam key   = TypeParam(index);
			const TypeParam value = TypeParam(index + 1UL);
			EXPECT_TRUE(h.Contains(key));
			EXPECT_EQ(h[key], value);
		}
	}

	TYPED_TEST_P(HashMapTest, Remove)
	{
		HashMap h{ Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) };
		EXPECT_FALSE(h.IsEmpty());
		EXPECT_EQ(h.Size(), 3UL);
		EXPECT_EQ(h.ByteSize(), h.Size() * sizeof(typename decltype(h)::KeyValueType));
		EXPECT_EQ(h.Capacity(), decltype(h)::k_initial_bucket_count);

		// Remove (const KeyType&)
		{
			TypeParam key = TypeParam(1);
			h.Remove(key);
			EXPECT_EQ(h.Size(), 2UL);
			EXPECT_EQ(h.ByteSize(), h.Size() * sizeof(typename decltype(h)::KeyValueType));
			EXPECT_EQ(h.Capacity(), decltype(h)::k_initial_bucket_count);
			ASSERT_FALSE(h.Contains(key));
		}

		// Remove (KeyType&&)
		{
			TypeParam key = TypeParam(2);
			h.Remove(Move(key));
			EXPECT_EQ(h.Size(), 1UL);
			EXPECT_EQ(h.ByteSize(), h.Size() * sizeof(typename decltype(h)::KeyValueType));
			EXPECT_EQ(h.Capacity(), decltype(h)::k_initial_bucket_count);
			ASSERT_FALSE(h.Contains(TypeParam(2)));
		}
	}

	TYPED_TEST_P(HashMapTest, Remove_NonExistentKey)
	{
		HashMap h{ Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) };
		EXPECT_FALSE(h.IsEmpty());
		EXPECT_EQ(h.Size(), 3UL);
		EXPECT_EQ(h.ByteSize(), h.Size() * sizeof(typename decltype(h)::KeyValueType));
		EXPECT_EQ(h.Capacity(), decltype(h)::k_initial_bucket_count);

		TypeParam non_existent_key = TypeParam(3);
		ASSERT_FALSE(h.Contains(non_existent_key));

		h.Remove(non_existent_key);
		EXPECT_EQ(h.Size(), 3UL);
		EXPECT_EQ(h.ByteSize(), h.Size() * sizeof(typename decltype(h)::KeyValueType));
		EXPECT_EQ(h.Capacity(), decltype(h)::k_initial_bucket_count);
	}

	TYPED_TEST_P(HashMapTest, Find)
	{
		HashMap h{ Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) };
		EXPECT_FALSE(h.IsEmpty());
		EXPECT_EQ(h.Size(), 3UL);
		EXPECT_EQ(h.ByteSize(), h.Size() * sizeof(typename decltype(h)::KeyValueType));
		EXPECT_EQ(h.Capacity(), decltype(h)::k_initial_bucket_count);

		{
			using ConstPointerType = decltype(h)::ConstPointerType;
			TypeParam key          = TypeParam(1);
			ASSERT_TRUE(h.Contains(key));
			ConstPointerType result = h.Find(key);
			ASSERT_TRUE(result);
			EXPECT_EQ(*result, TypeParam(2));
		}

		{
			using PointerType      = decltype(h)::PointerType;
			TypeParam existing_key = TypeParam(2);
			ASSERT_TRUE(h.Contains(existing_key));
			PointerType result = h.Find(existing_key);
			ASSERT_TRUE(result);
			EXPECT_EQ(*result, TypeParam(3));
		}
	}

	TYPED_TEST_P(HashMapTest, Find_NonExistentKey)
	{
		HashMap h{ Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) };
		EXPECT_FALSE(h.IsEmpty());
		EXPECT_EQ(h.Size(), 3UL);
		EXPECT_EQ(h.ByteSize(), h.Size() * sizeof(typename decltype(h)::KeyValueType));
		EXPECT_EQ(h.Capacity(), decltype(h)::k_initial_bucket_count);

		{
			using ConstPointerType = decltype(h)::ConstPointerType;
			TypeParam key          = TypeParam(3);
			ASSERT_FALSE(h.Contains(key));
			ConstPointerType result = h.Find(key);
			ASSERT_FALSE(result);
		}

		{
			using PointerType      = decltype(h)::PointerType;
			TypeParam existing_key = TypeParam(4);
			ASSERT_FALSE(h.Contains(existing_key));
			PointerType result = h.Find(existing_key);
			ASSERT_FALSE(result);
		}
	}

	TYPED_TEST_P(HashMapTest, Clear)
	{
		HashMap h{ Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) };
		EXPECT_FALSE(h.IsEmpty());
		EXPECT_EQ(h.Size(), 3UL);
		EXPECT_EQ(h.ByteSize(), h.Size() * sizeof(typename decltype(h)::KeyValueType));
		EXPECT_EQ(h.Capacity(), decltype(h)::k_initial_bucket_count);

		h.Clear();

		EXPECT_TRUE(h.IsEmpty());
		EXPECT_EQ(h.Size(), 0UL);
		EXPECT_EQ(h.ByteSize(), h.Size() * sizeof(typename decltype(h)::KeyValueType));
		EXPECT_EQ(h.Capacity(), decltype(h)::k_initial_bucket_count);
	}

	TYPED_TEST_P(HashMapTest, LoadFactor)
	{
		static constexpr Array k_expected_load_factors = { 0.0, 0.0625, 0.125, 0.1875 };

		HashMap<TypeParam, TypeParam> h{};
		EXPECT_TRUE(h.IsEmpty());
		EXPECT_EQ(h.Size(), 0UL);
		EXPECT_EQ(h.ByteSize(), h.Size() * sizeof(typename decltype(h)::KeyValueType));
		EXPECT_EQ(h.Capacity(), decltype(h)::k_initial_bucket_count);

		for (USize index = 0; index < k_expected_load_factors.Size() - 1; ++index) {
			EXPECT_EQ(h.LoadFactor(), k_expected_load_factors[index]);
			h.Insert(TypeParam(index), TypeParam(index + 1UL));
			EXPECT_EQ(h.LoadFactor(), k_expected_load_factors[index + 1]);
		}
	}

	REGISTER_TYPED_TEST_SUITE_P(HashMapTest,
	                            Constructor_InitialCapacity,
	                            Constructor_InitializerList_Empty,
	                            Constructor_InitializerList,
	                            Constructor_Copy,
	                            Constructor_Move,
	                            Operator_Copy,
	                            Operator_Move,
	                            Operator_Subscript,
	                            Iterator_BeginEnd_NonConst,
	                            Iterator_BeginEnd_Const,
	                            Insert,
	                            Insert_ManyValuesCausingRehash,
	                            Remove,
	                            Remove_NonExistentKey,
	                            Find,
	                            Find_NonExistentKey,
	                            Clear,
	                            LoadFactor);

	INSTANTIATE_TYPED_TEST_SUITE_P(HashMapPrimitiveTypesTest, HashMapTest, PrimitiveTypes);
	INSTANTIATE_TYPED_TEST_SUITE_P(HashMapComplexTypesTest, HashMapTest, ComplexType);
}  // namespace Orion::Engine::UT
