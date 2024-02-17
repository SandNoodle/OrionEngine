#pragma once

#include "platform.h"
#include "platform/types.h"

namespace orion
{
	/**
	 * @brief Allocates new memory block of a given size.
	 *
	 * @param size The size of the allocation (in bytes).
	 * @return Pointer to an allocated memory block.
	 */
	void* platform_allocate(size_t n);

	/**
	 * @brief Reallocates given memory block with new_size.
	 *
	 * @param block Pointer to a memory block.
	 * @param new_size The new size of the allocation (in bytes).
	 * @return Pointer to a reallocated memory block.
	 */
	void* platform_reallocate(void* block, size_t new_size);

	/**
	 * @brief Frees the given memory block.
	 *
	 * @param block The block to be freed.
	 */
	void platform_free(void* block);

	/**
	 * @brief Zeroes out the given block of memory.
	 *
	 * @param block The block to be zeroed out.
	 * @param size Size of the memory block.
	 * @return Pointer to the zeored out memory block.
	 */
	void* platform_zero_memory(void* block, size_t size);

	/**
	 * @brief Copies bytes of given size from source memory block to destination.
	 *
	 * @param dst Pointer to destination memory block.
	 * @param src Pointer to source memory block.
	 * @param size Size of the data to be copied.
	 * @return Pointer to the destination memory block.
	 */
	void* platform_copy_memory(void* dst, const void* src, size_t size);

	/**
	 * @brief Sets bytes of the given memory block to a value.
	 *
	 * @param dst Pointer to destination memory block.
	 * @param value The value to be set.
	 * @param size Size of the data to be set.
	 * @return
	 */
	void* platform_set_memory(void* dst, i32 value, size_t size);
}

