#pragma once

namespace orion
{
	struct allocator_t;

	/**
	 * @brief Dummy alllocator is an allocator that
	 * does not allocate nor free any memory.
	 * It is useful when tesking if code handles
	 * null pointers well.
	 *
	 * @detail When allocating it returns only nullptrs.
	 *
	 */
	allocator_t dummy_allocator_create(void);
}
