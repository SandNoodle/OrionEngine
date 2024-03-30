#include "arena_allocator.h"

#include "platform/compiler_macros.h"
#include "platform/memory.h"
#include "core/assert.h"
#include "core/log.h"
#include "core/math/omath.h"
#include "core/memory/allocators/allocator.h"
#include "orion_config.h"

namespace orion
{
	static uintptr_t align_forward(uintptr_t ptr, allocator_t::size_type alignment);

	struct arena_allocator_t
	{
		void*                  memory;
		allocator_t::size_type total_size;
		allocator_t::size_type current_size;
		b8                     is_extendable;
	};

	static void arena_create(allocator_t::size_type total_size, void* context)
	{
		arena_allocator_t* c = (arena_allocator_t*)context;

		const allocator_t::size_type total_size_aligned = math::next_power_of_two(total_size);

		OE_ASSERT_FALSE(c->memory, "arena_allocator_t: create() was already called.");
		c->memory            = platform_allocate(total_size_aligned);
		c->total_size        = total_size_aligned;
		c->current_size      = 0;
		OE_LOG_TRACE("arena_allocator_t: create() called, requesting the initial allocation of %zu bytes.", total_size);
	}

	static void arena_destroy(void* context)
	{
		if(!context)
		{
			OE_LOG_WARN("arena_allocator_t: tried to call destroy() a second time.");
			return;
		}

		arena_allocator_t* c = (arena_allocator_t*)context;
		OE_LOG_TRACE("arena_allocator_t: destroy() called, which freed all %zu bytes.", c->total_size);
		c->current_size = 0;
		c->total_size = 0;
		platform_free(c->memory);
		platform_free(c);
	}

	// TODO(sand_noodles): This function should do this things as follows:
	//                     1. Given that current_ptr = c->memory + c->current_size;
	//                     2. Given that aligned_ptr = align_forward(current_ptr, alignment);
	//                     3. Given that aligned_size = aligned_ptr - c->memory;
	//                     4. Given that requested_size = aligned_size + size;
	//                     5. If requested_size > c->total_size
	//                        0. Check if extendable, if not -> return nullptr;
	//                        1. new_total_size is equal to either requested_size or total_size * GROWTH_RATE,
	//                           both of these then rounded to the nearest power of two.
	//                        2.  Reallocate memory.
	//                        3. Check if reallocation succeeded, if not -> return nullptr;
	//                     6. c->current_size = requested_size;
	//                     7. Given that ptr = c->memory + aligned_size;
	//                     7. Return ptr;
	static void* arena_allocate_aligned(allocator_t::size_type size, allocator_t::size_type alignment, void* context)
	{
		arena_allocator_t* c = (arena_allocator_t*)context;

		uintptr_t current_ptr = (uintptr_t)c->memory + (uintptr_t)c->current_size;
		allocator_t::size_type aligned_size = align_forward(current_ptr, alignment) - (uintptr_t)c->memory;
		allocator_t::size_type requested_size = aligned_size + size;

		// Ensure capacity.
		if(requested_size > c->total_size)
		{
			if(!c->is_extendable)
			{
				OE_LOG_ERROR("arena_allocator_t: arena_allocate_aligned() failed to increase the capcity of the allocator, because its not extendable.");
				OE_LOG_WARN("arena_allocator_t: [HINT] try setting 'is_extendable' to 'true' when creating the allocator.");
				return nullptr;
			}

			const allocator_t::size_type new_total_size =
				math::next_power_of_two(
				      math::max(
				      requested_size,
				      c->total_size * ORION_ALLOCATORS_ARENA_CAPACITY_GROWTH_RATE
				));
			OE_LOG_TRACE("arena_allocator_t: increased the capacity from %zu to %zu bytes.", c->total_size, new_total_size);
			c->memory = platform_reallocate(c->memory, new_total_size);
			c->total_size = new_total_size;
			if(!c->memory)
			{
				OE_LOG_ERROR("arena_allocator_t: failed to reallocate the memory.", new_total_size);
				return nullptr;
			}
		}

		c->current_size = requested_size;
		void* ptr = (u8*)c->memory + aligned_size;
		OE_LOG_TRACE("arena_allocator_t: allocated %zu bytes (aligned to %zu bytes).", size, alignment);
		return ptr;
	}

	static void* arena_allocate(allocator_t::size_type size, void* context)
	{
		return arena_allocate_aligned(size,
			                          ORION_MEMORY_DEFAULT_ALIGNMENT,
			                          (arena_allocator_t*)context);
	}

	static void arena_deallocate(void* ptr, allocator_t::size_type size, void* context)
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
		OE_LOG_TRACE("arena_allocator_t: arena_deallocate_all() called.");
	}

	static void* arena_get_base_pointer(void* context)
	{
		return ((arena_allocator_t*)context)->memory;
	}

	static allocator_t::size_type arena_current_size(void* context)
	{
		return ((arena_allocator_t*)context)->current_size;
	}

	static allocator_t::size_type arena_max_size(void* context)
	{
		return ((arena_allocator_t*)context)->total_size;
	}

	allocator_t arena_allocator_create(b8 is_extendable)
	{
		arena_allocator_t* context = (arena_allocator_t*)platform_allocate(sizeof(arena_allocator_t)); 
		context->is_extendable = is_extendable;
		return (allocator_t) {
			.user_context = context,
			.create = arena_create,
			.destroy = arena_destroy,
			.allocate = arena_allocate,
			.allocate_aligned = arena_allocate_aligned,
			.deallocate = arena_deallocate,
			.deallocate_all = arena_deallocate_all,
			.get_base_pointer = arena_get_base_pointer,
			.get_current_size = arena_current_size,
			.get_max_size = arena_max_size,
		};
	}

	//
	// Private
	//

	static uintptr_t align_forward(uintptr_t ptr, allocator_t::size_type alignment)
	{
		OE_ASSERT_TRUE(is_power_of_two(alignment),
			"arena_allocator_t: align_forward() failed - alignment has to be a power of two.");

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
