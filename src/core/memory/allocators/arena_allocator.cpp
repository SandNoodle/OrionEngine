#include "arena_allocator.h"

#include "platform/compiler_macros.h"
#include "platform/memory.h"
#include "core/log.h"
#include "core/assert.h"
#include "orion_config.h"

// TODO(sand_noodles): Maybe this can be rewritten as a region based arena allocator?
namespace orion
{
	static uintptr_t align_forward(uintptr_t ptr, usize alignment);

	struct arena_allocator_t
	{
		void* memory;
		usize total_size;
		usize current_size;
	};

	static void arena_create(usize total_size, void* context)
	{
		arena_allocator_t* c = (arena_allocator_t*)context;
		c->memory            = platform_allocate(total_size);
		c->total_size        = total_size;
		c->current_size      = 0;
		platform_zero_memory(c->memory, total_size);
		OE_LOG_TRACE("arena_allocator_t: created with size of %zu", total_size);
	}

	static void arena_destroy(void* context)
	{
		if(!context)
			return;

		arena_allocator_t* c = (arena_allocator_t*)context;
		OE_LOG_TRACE("arena_allocator_t: destroyed and freed %zu bytes.", c->total_size);
		c->current_size = 0;
		c->total_size = 0;
		platform_free(c->memory);
		platform_free(c);
	}

	static void* arena_allocate_aligned(usize size, usize alignment, void* context)
	{
		arena_allocator_t* c = (arena_allocator_t*)context;

		uintptr_t current_ptr = (uintptr_t)c->memory + (uintptr_t)c->current_size;
		uintptr_t relative_offset = align_forward(current_ptr, alignment) - (uintptr_t)c->memory;

		if(relative_offset + size <= c->total_size)
		{
			void* ptr = ((u8*)c->memory) + relative_offset;
			c->current_size = relative_offset + size;
			OE_LOG_TRACE("arena_allocator_t: allocated %zu bytes (aligned to %zu bytes).", size, alignment);
			platform_zero_memory(ptr, size); // Zero memory by default.
			return ptr;
		}

		OE_LOG_ERROR("arena_allocator_t: allocation of %zu bytes failed - no available memory left (total_size: %zu bytes).", size, c->total_size);
		return nullptr;
	}

	static void* arena_allocate(usize size, void* context)
	{
		return arena_allocate_aligned(size,
			                          ORION_MEMORY_DEFAULT_ALIGNMENT,
			                          (arena_allocator_t*)context);
	}

	static void arena_deallocate(void* ptr, usize size, void* context)
	{
		ORION_SUPPRESS_UNUSED(ptr);
		ORION_SUPPRESS_UNUSED(size);
		ORION_SUPPRESS_UNUSED(context);
		/* Does nothing */
	}

	static void arena_deallocate_all(void* context)
	{
		arena_allocator_t* c = (arena_allocator_t*)context;
		c->current_size = 0;
		platform_zero_memory(c->memory, c->total_size);
	}

	static void* arena_get_base_pointer(void* context)
	{
		arena_allocator_t* c = (arena_allocator_t*)context;
		return c->memory;
	}

	allocator_t arena_allocator_create(void)
	{
		return (allocator_t) {
			.user_context = (arena_allocator_t*)platform_allocate(sizeof(arena_allocator_t)),
			.create = arena_create,
			.destroy = arena_destroy,
			.allocate = arena_allocate,
			.allocate_aligned = arena_allocate_aligned,
			.deallocate = arena_deallocate,
			.deallocate_all = arena_deallocate_all,
			.get_base_pointer = arena_get_base_pointer,
		};
	}

	//
	// Private
	//

	static uintptr_t align_forward(uintptr_t ptr, usize alignment)
	{
		OE_ASSERT_TRUE(is_power_of_two(alignment),
			"arena_allocator_t: alignment has to be a power of two.");

		uintptr_t p = ptr;
		uintptr_t a = (uintptr_t)alignment;
		uintptr_t modulo = p & (a - 1); // Same as modulo, but faster due to the alignment == 2^N.
		if(modulo != 0)
		{
			// Push the address to the next alignment.
			p += a - modulo;
		}
		return p;
	}
}
