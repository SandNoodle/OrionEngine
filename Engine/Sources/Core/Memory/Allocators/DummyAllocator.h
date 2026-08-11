#pragma once

#include "OrionEngine.h"

#include "Core/Memory/Allocators/Allocator.h"

namespace Orion::Engine::Memory
{
	/**
	 * @brief DummyAllocator is an allocator, which allocates and deallocates nothing.
	 * Useful, whenever allocator is required to be passed, but the memory aspect is not needed.
	 */
	class DummyAllocator
	{
		public:
		using SizeType = USize;

		public:
		[[nodiscard]] ORION_FORCE_INLINE constexpr void* Allocate(SizeType size_in_bytes, SizeType alignment) noexcept;
		ORION_FORCE_INLINE constexpr void Free(const void* ptr) noexcept;
		ORION_FORCE_INLINE constexpr void FreeAll() noexcept;
	};
	static_assert(AllocatorKind<DummyAllocator>,
	              "DummyAllocator does not meet the requirements of being an Allocator.");

	// -- Implementation.
	constexpr auto DummyAllocator::Allocate(SizeType size_in_bytes, SizeType alignment) noexcept -> void*
	{
		ORION_IGNORE_PARAM(size_in_bytes);
		ORION_IGNORE_PARAM(alignment);
		// Explicitly nothing - this allocator does not allocate any memory.
		return nullptr;
	}

	ORION_FORCE_INLINE constexpr auto DummyAllocator::Free(const void* ptr) noexcept -> void
	{
		ORION_IGNORE_PARAM(ptr);
		// Explicitly nothing - this allocator does not deallocate any memory.
	}

	ORION_FORCE_INLINE constexpr auto DummyAllocator::FreeAll() noexcept -> void
	{
		// Explicitly nothing - this allocator does not deallocate any memory.
	}

}  // namespace Orion::Engine::Memory
