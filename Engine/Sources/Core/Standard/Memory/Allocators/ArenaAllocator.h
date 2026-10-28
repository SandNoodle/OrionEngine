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
	/// @important Individual allocations cannot be deallocated - only freed at once.
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

		[[nodiscard]] void* Allocate(SizeType size_in_bytes, SizeType alignment) noexcept;
		void Free(void* ptr) noexcept;
		void FreeAll() noexcept;
	};
	static_assert(AllocatorKind<ArenaAllocator>,
	              "ArenaAllocator does not meet the requirements of being an Allocator.");
}  // namespace Orion::Engine::Memory
