#include "TestCommon.h"

#include "Core/Memory/Allocators/DummyAllocator.h"
#include "Core/Standard/Containers/HashMap.h"

namespace Orion::Engine::UT
{
	template <typename T>
	class HashMapTest : public ::testing::Test
	{
	};
	TYPED_TEST_SUITE(HashMapTest, PrimitiveTypes);

	TYPED_TEST(HashMapTest, DISABLED_Constructor_InitialCapacity)
	{
		using Type = TypeParam;
		HashMap h  = HashMap<Type, Type, Hash<Type>, Predicate<Type>, DummyAllocator>(8, DummyAllocator());
		EXPECT_TRUE(h.IsEmpty());
		EXPECT_EQ(h.Size(), 0);
		EXPECT_EQ(h.ByteSize(), 0);
		EXPECT_EQ(h.Capacity(), 8);
	}

	TYPED_TEST(HashMapTest, DISABLED_Constructor_InitializerList)
	{
		using Type = TypeParam;
		HashMap h  = HashMap<Type, Type, Hash<Type>, Predicate<Type>, DummyAllocator>({});
		EXPECT_TRUE(h.IsEmpty());
		EXPECT_EQ(h.Size(), 0);
		EXPECT_EQ(h.ByteSize(), 0);
		EXPECT_EQ(h.Capacity(), 8);
	}
}  // namespace Orion::Engine::UT
