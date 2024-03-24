#pragma once

#include "platform/types.h"

namespace orion
{
	/**
	 * @brief @TODO descriptions
	 */
	struct allocator_t
	{
		typedef usize size_type;

		void* user_context;

		/**
		 * @brief Prepares the allocator for the initial use.
		 *
		 * @param capacity initial capacity of the allocator.
		 * @param context pointer to the allocator's context structure.
		 */
		void (*create)(size_type initial_capacity, void* context);

		/**
		 * @brief Destroys the given allocator, cleaning it up and freeing all the memory held.
		 * @param context pointer to the allocator's context structure.
		 */
		void (*destroy)(void* context);

		/**
		 * @brief Performs an allocation given size.
		 * @param size size in bytes to allocate.
		 * @param context pointer to the allocator's context structure.
		 * @return pointer to newly allocated memory or nullptr if allocation failed.
		 */
		void* (*allocate)(size_type size, void* context);

		/**
		 * @brief Performs an allocation of given size, which
		 * is aligned to a given power of two.
		 * @param size size in bytes to allocate.
		 * @param context pointer to the allocator's context structure.
		 * @return pointer to newly allocated memory or nullptr if allocation failed.
		 */
		void* (*allocate_aligned)(size_type size, size_type alignment, void* context);

		/**
		 * @brief Frees the given block of memory.
		 * @param ptr Pointer to the block of memory to be freed.
		 * @param size size in bytes of the block.
		 * @param context pointer to the allocator's context structure.
		 */
		void (*deallocate)(void* ptr, size_type size, void* context);

		/**
		 * @brief Frees all the memory held by the allocator.
		 * @param context pointer to the allocator's context structure.
		 */
		void (*deallocate_all)(void* context);

		/**
		 * @brief Returns the pointer to memory at the start of the allocator.
		 * @param context pointer to the allocator's context structure.
		 * @return pointer to the memory 'at an offset equal to 0'.
		 */
		void* (*get_base_pointer)(void* context);
	};
}
