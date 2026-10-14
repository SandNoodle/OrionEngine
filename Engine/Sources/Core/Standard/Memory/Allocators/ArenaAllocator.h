#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Memory/Allocators/Allocator.h"

namespace Orion::Engine::Memory
{
	namespace Detail
	{
		/// @brief Represents a single memory region block for performing individual allocations and deallocations with
		/// pointer to the next one (in case of it being full).
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

	/// @brief ArenaAllocator (also known as LinearAllocator) is an allocator, which allocates and deallocates memory by
	/// bumping a single internal pointer.
	/// @important Individual allocations cannot be deallocated - only freed at once.
	class ArenaAllocator
	{
		public:
		using ThisType = ArenaAllocator;
		using SizeType = Detail::ArenaRegion::SizeType;

		public:
		constexpr ArenaAllocator();
		constexpr ArenaAllocator(const ThisType&) = delete;
		constexpr ArenaAllocator(ThisType&&)      = default;
		constexpr ~ArenaAllocator();

		constexpr ArenaAllocator& operator=(const ThisType&) = delete;
		constexpr ArenaAllocator& operator=(ThisType&&)      = default;

		[[nodiscard]] ORION_FORCE_INLINE constexpr void* Allocate(SizeType size_in_bytes, SizeType alignment) noexcept;
		ORION_FORCE_INLINE constexpr void Free(void* ptr) noexcept;
		ORION_FORCE_INLINE constexpr void FreeAll() noexcept;
	};
	static_assert(AllocatorKind<ArenaAllocator>,
	              "ArenaAllocator does not meet the requirements of being an Allocator.");

	// -- Implementation.
	constexpr ArenaAllocator::ArenaAllocator() {}

	constexpr ArenaAllocator::~ArenaAllocator() {}

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
