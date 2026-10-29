#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Handle.h"
#include "Core/Standard/Memory/Allocators/Allocator.h"

namespace Orion::Engine::Memory
{
	/// @brief Represents (internal) continuous allocated block of memory.
	ORION_DEFINE_OPAQUE_HANDLE(ArenaRegion);

	/// @brief ArenaAllocator (also known as LinearAllocator) is an allocator, which allocates and deallocates memory by
	/// bumping a single internal pointer.
	/// @warning Individual allocations cannot be deallocated - only freed at once.
	class ArenaAllocator
	{
		public:
		using ThisType = ArenaAllocator;
		using SizeType = USize;

		static constexpr SizeType k_default_block_size = ORION_MEMORY_ALLOCATORS_ARENA_ALLOCATOR_DEFAULT_BLOCK_SIZE;

		private:
		ArenaRegionHandle _regions_begin;
		ArenaRegionHandle _regions_current;

		public:
		ArenaAllocator();
		ArenaAllocator(const ThisType&) = delete;
		ArenaAllocator(ThisType&&)      = default;
		~ArenaAllocator();

		ArenaAllocator& operator=(const ThisType&) = delete;
		ArenaAllocator& operator=(ThisType&&)      = default;

		/// @brief Allocates \p size_in_bytes sized block of contiguous memory.
		/// @param[IN, REQUIRED] size_in_bytes Amount of bytes to allocate.
		/// @param[IN, OPTIONAL] alignment Alignment to use.
		/// @return Returns pointer to the allocated block of memory on success, nullptr otherwise.
		[[nodiscard]] void* Allocate(SizeType size_in_bytes,
		                             SizeType alignment = ORION_MEMORY_DEFAULT_ALIGNMENT) noexcept;

		/// @brief Frees previously allocated block of memory.
		/// @warning \p ptr must NOT be null!
		/// @warning Effectively does nothing - ArenaAllocator does not support freeing individual allocations, only all
		/// of them at once. This function exists only for completeness of the interface.
		/// @param[IN, REQUIRED] ptr Pointer to the data.
		void Free(void* ptr) noexcept;

		/// @brief Frees all previously allocated blocks of memory.
		/// @warning Effectively sets the size of each ArenaRegion to 0.
		/// @warning Does not de-allocate memory blocks!
		void FreeAll() noexcept;

		/// @brief Returns the amount of bytes that this container currently stores.
		[[nodiscard]] SizeType Size() const noexcept;

		/// @brief Returns the maximum amount of bytes that can be currently stored in this container.
		[[nodiscard]] SizeType Capacity() const noexcept;
	};
	static_assert(AllocatorKind<ArenaAllocator>,
	              "ArenaAllocator does not meet the requirements of being an Allocator.");
}  // namespace Orion::Engine::Memory
