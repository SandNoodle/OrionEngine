#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Memory/Allocators/Allocator.h"

namespace Orion::Engine::Memory
{
	namespace Detail
	{
		/// @brief TODO
		struct ArenaRegion
		{
			public:
			using SizeType = USize;

			public:
			ArenaRegion* next;
			Byte* data{ nullptr };
			SizeType offset_current{ 0UL };
			SizeType offset_previous{ 0UL };
			SizeType capacity{ 0UL };
		};
	}  // namespace Detail

	/// @brief TODO
	class ArenaAllocator
	{
		public:
		using SizeType = Detail::ArenaRegion::SizeType;

		public:
		[[nodiscard]] ORION_FORCE_INLINE constexpr void* Allocate(SizeType size_in_bytes, SizeType alignment) noexcept;
		ORION_FORCE_INLINE constexpr void Free(void* ptr) noexcept;
		ORION_FORCE_INLINE constexpr void FreeAll() noexcept;
	};
	static_assert(AllocatorKind<ArenaAllocator>,
	              "ArenaAllocator does not meet the requirements of being an Allocator.");

	// -- Implementation.
	ORION_FORCE_INLINE constexpr auto ArenaAllocator::Allocate(SizeType size_in_bytes, SizeType alignment) noexcept
		-> void*
	{
		ORION_IGNORE_PARAM(size_in_bytes);
		ORION_IGNORE_PARAM(alignment);
		ORION_NOT_IMPLEMENTED();
	}

	ORION_FORCE_INLINE constexpr auto ArenaAllocator::Free(void* ptr) noexcept -> void
	{
		ORION_IGNORE_PARAM(ptr);
		/// NOTE: ArenaAllocator has no concept of freeing invididual allocations.
	}

	ORION_FORCE_INLINE constexpr auto ArenaAllocator::FreeAll() noexcept -> void
	{
		ORION_NOT_IMPLEMENTED();
	};
}  // namespace Orion::Engine::Memory
