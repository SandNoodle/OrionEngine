#pragma once

#include "platform/types.h"

#include "core/memory/allocators/allocator.h"

namespace orion
{
	/**
	 * @brief Arena allocator is an allocator that allocates
	 * memory in a linear fashion, by simply moving
	 * internal pointer by a allocation of given size.
	 * However memory can be deallocated only all at once.
	 *
	 * @param is_extendable should arena allocator be capable
	 * of extending its capacity.
	 */
	allocator_t arena_allocator_create(b8 is_extendable);
}
