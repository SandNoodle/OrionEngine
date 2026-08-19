#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Memory/Allocators/Allocator.h"
#include "Platform/Memory.h"

namespace Orion::Engine::Memory
{
	namespace Detail
	{
		struct Arena
		{
			public:
			using SizeType = USize;

			public:
			Arena* next{ nullptr };
			void* data{ nullptr };
			SizeType offset_current{ 0UL };
			SizeType offset_previous{ 0UL };
			SizeType capacity{ 0UL };
		};
	}  // namespace Detail

	/**
	 * @brief ArenaAllocator
	 */
	class ArenaAllocator
	{
		public:
		using SizeType = Detail::Arena::SizeType;

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

	ORION_FORCE_INLINE constexpr auto ArenaAllocator::Free(void* ptr) noexcept -> void {}

	ORION_FORCE_INLINE constexpr auto ArenaAllocator::FreeAll() noexcept -> void {};
}  // namespace Orion::Engine::Memory
