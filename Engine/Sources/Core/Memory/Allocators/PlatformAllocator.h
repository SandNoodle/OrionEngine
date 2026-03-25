#pragma once

#include "OrionEngine.h"

#include "Core/Memory/Allocators/Allocator.h"
#include "Platform/Platform.h"

namespace Orion::Engine
{
	/// @brief PlatformAllocator is a simple allocator, which allocates and deallocates memory by calling given
	/// platform's API.
	/// @important These calls might result in a significant latency overhead - use sparingly and with caution.
	class PlatformAllocator
	{
		public:
		using SizeType = USize;

		public:
		[[nodiscard]] ORION_FORCE_INLINE constexpr void* Allocate(SizeType size_in_bytes, SizeType alignment) noexcept;

		ORION_FORCE_INLINE constexpr void Free(void* ptr) noexcept;

		ORION_FORCE_INLINE constexpr void FreeAll() noexcept;
	};
	static_assert(AllocatorKind<PlatformAllocator>,
	              "PlatformAllocator does not meet the requirements of being an Allocator.");

	// -- Implementation.
	constexpr auto PlatformAllocator::Allocate(SizeType size_in_bytes, SizeType alignment) noexcept -> void*
	{
		// TODO(SandNoodle): Alignment based allocation.
		ORION_IGNORE_PARAM(alignment);
		return Platform::MemoryAllocate(size_in_bytes);
	}

	ORION_FORCE_INLINE constexpr auto PlatformAllocator::Free(void* ptr) noexcept -> void
	{
		return Platform::MemoryFree(ptr);
	}

	ORION_FORCE_INLINE constexpr auto PlatformAllocator::FreeAll() noexcept -> void
	{
		// Explicitly nothing - this allocator does not hold any state.
	}

}  // namespace Orion::Engine
