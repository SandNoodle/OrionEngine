#include "core/memory/allocators/dummy_allocator.h"

#include "core/memory/allocators/allocator.h"

namespace orion
{

	static void  dummy_create(allocator_t::size_type, void*) {}
	static void  dummy_destroy(void*) {}
	static void* dummy_allocate(allocator_t::size_type, void*) { return nullptr; }
	static void* dummy_allocate_aligned(allocator_t::size_type, allocator_t::size_type, void*) { return nullptr; }
	static void  dummy_deallocate(void*, allocator_t::size_type, void*) {}
	static void  dummy_deallocate_all(void*) {}
	static void* dummy_get_base_pointer(void*) {return nullptr; }
	static allocator_t::size_type dummy_size (void*) { return 0; }

	allocator_t dummy_allocator_create(void)
	{
		return (allocator_t) {
			.user_context = nullptr,
			.create = dummy_create,
			.destroy = dummy_destroy,
			.allocate = dummy_allocate,
			.allocate_aligned = dummy_allocate_aligned,
			.deallocate = dummy_deallocate,
			.deallocate_all = dummy_deallocate_all,
			.get_base_pointer = dummy_get_base_pointer,
			.get_current_size = dummy_size,
			.get_max_size = dummy_size,
		};
	}
}
