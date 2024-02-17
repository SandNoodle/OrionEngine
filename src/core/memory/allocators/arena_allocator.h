#pragma once

#include "platform/types.h"

namespace orion
{
	/**
	 * @brief Arena allocator is an allocator that allocates
	 * memory in a linear fashion, by simply moving
	 * internal pointer by a allocation of given size.
	 * However memory can be deallocated only all at once.
	 */
	typedef struct arena_allocator_t arena_allocator_t;

	/**
	 * @brief Creates arena allocator of given size.
	 * @param total size of an arena allocator.
	 * @return pointer to newly created arena_allocator.
	 */
	arena_allocator_t* arena_allocator_create(usize size);

	/** @brief Destroys given arena allocator. */
	void arena_allocator_destroy(arena_allocator_t* allocator);

	/**
	 * @brief Performs an allocation of given size, which
	 * is aligned to a given power of two alignment.
	 * @param allocator arena allocator that will return the memory from.
	 * @param size size in bytes to allocate.
	 * @param alignment power of two's alignment.
	 * @return pointer to newly allocated memory or nullptr if allocation failed.
	 */
	void* arena_allocator_allocate_aligned(arena_allocator_t* allocator, usize size, usize alignment);

	/**
	 * @brief Performs an allocation of given size.
	 * @param allocator arena allocator that will return the memory from.
	 * @param size size in bytes to allocate.
	 * @return pointer to newly allocated memory or nullptr if allocation failed.
	 */
	void* arena_allocator_allocate(arena_allocator_t* allocator, usize size);

	/**
	 * @brief Arena allocator by its nature only deallocates memory all at once.
	 * This function does nothing, but is here to conform to the interface of a memory allocator.
	 */
	void arena_allocator_deallocate(arena_allocator_t* allocator, usize size);

	/**
	 * @brief Deallocate all the memory from given arena allocator.
	 * @param allocator arena allocator that will return the memory from.
	 */
	void arena_allocator_deallocate_all(arena_allocator_t* allocator);
}
