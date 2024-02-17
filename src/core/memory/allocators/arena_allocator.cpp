#include "arena_allocator.h"

#include "orion_config.h"

#include "platform/memory.h"

#include "core/log.h"
#include "core/assert.h"

// TODO: Maybe create region based arena allocator?
namespace orion
{
	struct arena_allocator_t
	{
		void* memory;
		size_t total_size;
		size_t current_size;
	};

	arena_allocator_t* arena_allocator_create(size_t size)
	{
		arena_allocator_t* a = (arena_allocator_t*)platform_allocate(sizeof(arena_allocator_t));
		a->memory = platform_allocate(size);
		a->total_size = size;
		a->current_size = 0;
		platform_zero_memory(a->memory, size);
		OE_LOG_TRACE("arena_allocator_create called with size of %zu", size);
		return a;
	}

	void arena_allocator_destroy(arena_allocator_t* allocator)
	{
		if(!allocator) return;
		allocator->current_size = 0;
		allocator->total_size = 0;
		platform_free(allocator->memory);
		platform_free(allocator);
		OE_LOG_TRACE("arena_allocator_destroy called.");
	}

	void* arena_allocator_allocate_aligned(arena_allocator_t* a, size_t size, size_t alignment)
	{
		if(!a) return nullptr;

		OE_ASSERT_TRUE(is_power_of_two(alignment),
			"arena_allocatort_allocate_aligned's alignment has to be a power of two.");

		size_t aligned_size = size & (alignment - 1);
		if(a->current_size + aligned_size <= a->total_size)
		{
			void* ptr = ((u8*)a->memory) + aligned_size;
			a->current_size += aligned_size;
			OE_LOG_TRACE("arena_allocator allocated %s bytes.", aligned_size);
			return ptr;
		}

		OE_LOG_ERROR("arena_allocator has run out of available memory.");
		return nullptr;
	}

	void* arena_allocator_allocate(arena_allocator_t* a, size_t size)
	{
		if(!a) return nullptr;
		return arena_allocator_allocate_aligned(a, size, ORION_MEMORY_DEFAULT_ALIGNMENT);
	}

	void arena_allocator_deallocate(arena_allocator_t* a, size_t size) { /* Does nothing */ }

	void arena_allocator_deallocate_all(arena_allocator_t* a)
	{
		if(!a) return;
		a->current_size = 0;
		platform_zero_memory(a->memory, a->total_size);
	}
}
