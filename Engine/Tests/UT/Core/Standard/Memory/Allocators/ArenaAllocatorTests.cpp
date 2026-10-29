#include "TestCommon.h"

#include "Core/Standard/Memory/Allocators/ArenaAllocator.h"

#include "Core/Standard/Containers/Array.h"

namespace Orion::Engine::Memory::UT
{
	namespace
	{
		class ArenaAllocatorTest : public ::testing::Test
		{
			protected:
			ArenaAllocator _allocator;
		};
	}  // namespace

	TEST_F(ArenaAllocatorTest, SmallAllocations_FittingInSingleDefaultBlock)
	{
		static constexpr Array k_allocation_sizes = { 12UL, 32UL, 64UL, 128UL };

		USize expected_allocated_size = 0UL;
		for (USize index = 0UL; index < k_allocation_sizes.Size(); ++index) {
			USize allocation_size = k_allocation_sizes[index];

			expected_allocated_size += allocation_size;
			void* result = _allocator.Allocate(allocation_size);
			EXPECT_TRUE(result);
			EXPECT_GE(_allocator.Size(), expected_allocated_size);
			EXPECT_EQ(_allocator.Capacity(), ArenaAllocator::k_default_block_size);

			// NOTE: ArenaAllocator does not support freeing individual allocations; pointer should stay valid.
			_allocator.Free(result);
			EXPECT_TRUE(result);
			EXPECT_GE(_allocator.Size(), expected_allocated_size);
			EXPECT_EQ(_allocator.Capacity(), ArenaAllocator::k_default_block_size);
		}

		_allocator.FreeAll();
		EXPECT_EQ(_allocator.Size(), 0UL);
		EXPECT_EQ(_allocator.Capacity(), ArenaAllocator::k_default_block_size);
	}

	TEST_F(ArenaAllocatorTest, SmallAllocations_OverflowIntoNewBlock)
	{
		static constexpr Array k_allocation_sizes = {
			512UL,
			32UL,
			ArenaAllocator::k_default_block_size - 1,
		};
		static constexpr Array k_expected_capacity = {
			ArenaAllocator::k_default_block_size,
			ArenaAllocator::k_default_block_size,
			ArenaAllocator::k_default_block_size * 2UL,
		};
		ASSERT_EQ(k_allocation_sizes.Size(), k_expected_capacity.Size());

		USize expected_allocated_size = 0UL;
		for (USize index = 0UL; index < k_allocation_sizes.Size(); ++index) {
			USize allocation_size = k_allocation_sizes[index];

			expected_allocated_size += allocation_size;
			void* result = _allocator.Allocate(allocation_size);
			EXPECT_TRUE(result);
			EXPECT_GE(_allocator.Size(), expected_allocated_size);
			EXPECT_EQ(_allocator.Capacity(), k_expected_capacity[index]);

			// NOTE: ArenaAllocator does not support freeing individual allocations; pointer should stay valid.
			_allocator.Free(result);
			EXPECT_TRUE(result);
			EXPECT_GE(_allocator.Size(), expected_allocated_size);
			EXPECT_EQ(_allocator.Capacity(), k_expected_capacity[index]);
		}

		_allocator.FreeAll();
		EXPECT_EQ(_allocator.Size(), 0UL);
		EXPECT_EQ(_allocator.Capacity(), k_expected_capacity.Back());
	}

	TEST_F(ArenaAllocatorTest, LargeAllocations_OverDefaultBlockSize)
	{
		static constexpr USize k_allocation_size = ArenaAllocator::k_default_block_size * 4;

		void* result = _allocator.Allocate(k_allocation_size);
		EXPECT_TRUE(result);
		EXPECT_GE(_allocator.Size(), k_allocation_size);
		EXPECT_EQ(_allocator.Capacity(), k_allocation_size);

		// NOTE: ArenaAllocator does not support freeing individual allocations; pointer should stay valid.
		_allocator.Free(result);
		EXPECT_TRUE(result);
		EXPECT_GE(_allocator.Size(), k_allocation_size);
		EXPECT_EQ(_allocator.Capacity(), k_allocation_size);

		_allocator.FreeAll();
		EXPECT_EQ(_allocator.Size(), 0UL);
		EXPECT_EQ(_allocator.Capacity(), k_allocation_size);
	}

	TEST_F(ArenaAllocatorTest, LargeAllocations_FindFittingBlock)
	{
		static constexpr Array k_initial_allocations = {
			128UL,
			ArenaAllocator::k_default_block_size - 32UL,
		};
		static constexpr Array k_expected_capacity = {
			ArenaAllocator::k_default_block_size,
			ArenaAllocator::k_default_block_size * 2UL,
		};
		ASSERT_EQ(k_initial_allocations.Size(), k_expected_capacity.Size());

		USize expected_allocated_size = 0UL;
		for (USize index = 0UL; index < k_initial_allocations.Size(); ++index) {
			USize allocation_size = k_initial_allocations[index];

			expected_allocated_size += allocation_size;
			void* result = _allocator.Allocate(allocation_size);
			EXPECT_TRUE(result);
			EXPECT_EQ(_allocator.Size(), expected_allocated_size);
			EXPECT_EQ(_allocator.Capacity(), k_expected_capacity[index]);

			// NOTE: ArenaAllocator does not support freeing individual allocations; pointer should stay valid.
			_allocator.Free(result);
			EXPECT_TRUE(result);
			EXPECT_EQ(_allocator.Size(), expected_allocated_size);
			EXPECT_EQ(_allocator.Capacity(), k_expected_capacity[index]);
		}

		// NOTE: Two memory blocks should now be allocated, both of k_default_block_size.
		EXPECT_EQ(_allocator.Capacity(), ArenaAllocator::k_default_block_size * 2UL);

		// Allocating exactly the amount of bytes to fill the first block should NOT cause allocation of a new one.
		// Even though it won't fit in the current block, first one should be used for it instead.
		{
			USize filling_allocation = ArenaAllocator::k_default_block_size - k_initial_allocations[0];
			expected_allocated_size += filling_allocation;
			void* result = _allocator.Allocate(filling_allocation);
			EXPECT_TRUE(result);
			EXPECT_EQ(_allocator.Size(), expected_allocated_size);
			EXPECT_EQ(_allocator.Capacity(), ArenaAllocator::k_default_block_size * 2UL);
		}

		// Allocating exactly the amount of bytes to fill the second block should NOT cause allocation of a new one.
		{
			USize filling_allocation = ArenaAllocator::k_default_block_size - k_initial_allocations[1];
			expected_allocated_size += filling_allocation;
			void* result = _allocator.Allocate(filling_allocation);
			EXPECT_TRUE(result);
			EXPECT_EQ(_allocator.Size(), expected_allocated_size);
			EXPECT_EQ(_allocator.Capacity(), ArenaAllocator::k_default_block_size * 2UL);
		}

		// Allocating 1 byte of data SHOULD cause an allocation of a new block - there's no capacity left in both first
		// and the second block.
		{
			expected_allocated_size += ORION_MEMORY_DEFAULT_ALIGNMENT;
			void* result = _allocator.Allocate(1UL);
			EXPECT_TRUE(result);
			EXPECT_EQ(_allocator.Size(), expected_allocated_size);
			EXPECT_EQ(_allocator.Capacity(), ArenaAllocator::k_default_block_size * 3UL);
		}

		_allocator.FreeAll();
		EXPECT_EQ(_allocator.Size(), 0UL);
		EXPECT_EQ(_allocator.Capacity(), ArenaAllocator::k_default_block_size * 3UL);
	}
}  // namespace Orion::Engine::Memory::UT
