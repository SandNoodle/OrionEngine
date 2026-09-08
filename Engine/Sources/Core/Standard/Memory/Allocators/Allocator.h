#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Concepts.h"

namespace Orion::Engine::Memory
{
	/// @brief Concept that is satisfied by a type that meets the requirements of being a Memory Allocator.
	template <typename T>
	concept AllocatorKind = requires(T allocator, USize size_in_bytes, USize alignment, void* ptr) {
		{ allocator.Allocate(size_in_bytes, alignment) } -> SameAs<void*>;
		{ allocator.Free(ptr) } -> SameAs<void>;
		{ allocator.FreeAll() } -> SameAs<void>;
	};

	/// @brief Helper function that performs type-sized allocation fo a single element.
	/// @tparam T (sizeof) Type to be allocated.
	/// @param[IN, REQUIRED] allocator Allocator which will perform the allocation.
	/// @param[IN, OPTIONAL] alignment Alignment to use.
	template <typename T, typename SizeType = USize>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T* Allocate(AllocatorKind auto& allocator,
	                                                       SizeType alignment = alignof(T))
	{
		return static_cast<T*>(allocator.Allocate(sizeof(T), alignment));
	}

	/// @brief Helper function that performs type-sized allocation of \p count elements.
	/// @tparam T (sizeof) Type to be allocated.
	/// @param[IN, REQUIRED] allocator Allocator which will perform the allocation.
	/// @param[IN, REQUIRED] count Amount of the elements to allocate.
	/// @param[IN, OPTIONAL] alignment Alignment of a single element.
	template <typename T, typename SizeType = USize>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T* AllocateCount(AllocatorKind auto& allocator,
	                                                            SizeType count,
	                                                            SizeType alignment = alignof(T)) noexcept
	{
		return static_cast<T*>(allocator.Allocate(sizeof(T) * count, alignment));
	}

	/// @brief Helper function that performs type-sized free of an allocated memory region.
	/// @tparam T (sizeof) Type to be freed.
	/// @param[IN, REQUIRED] allocator Allocator which will perform the allocation.
	/// @param[IN, REQUIRED] ptr Pointer to a memory region to be freed.
	template <typename T>
	ORION_FORCE_INLINE constexpr void Free(AllocatorKind auto& allocator, T* ptr)
	{
		allocator.Free(ptr);
	}

}  // namespace Orion::Engine::Memory
