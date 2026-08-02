#pragma once

#include "OrionEngine.h"

#include "Core/Memory/Allocators/Allocator.h"
#include "Platform/Memory.h"

namespace Orion::Engine::Memory
{
	class ArenaAllocator
	{
		public:
		using SizeType = USize;

		public:
		[[nodiscard]] ORION_FORCE_INLINE constexpr void* Allocate(SizeType size_in_bytes, SizeType alignment) noexcept;
		ORION_FORCE_INLINE constexpr void Free(void* ptr) noexcept;
		ORION_FORCE_INLINE constexpr void FreeAll() noexcept;
	};
	static_assert(AllocatorKind<ArenaAllocator>,
	              "PlatformAllocator does not meet the requirements of being an Allocator.");

	// -- Implementation.
	ORION_FORCE_INLINE constexpr auto ArenaAllocator::Allocate(SizeType size_in_bytes, SizeType alignment) noexcept
		-> void*
	{
	}

	ORION_FORCE_INLINE constexpr auto ArenaAllocator::Free(void* ptr) noexcept -> void {}

	ORION_FORCE_INLINE constexpr auto ArenaAllocator::FreeAll() noexcept -> void {};
}  // namespace Orion::Engine::Memory
