#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Concepts.h"

namespace Orion::Engine::Memory
{
	/// @brief TODO
	template <typename T>
	concept AllocatorKind = requires(T allocator, USize size_in_bytes, USize alignment, void* ptr) {
		{ allocator.Allocate(size_in_bytes, alignment) } -> SameAs<void*>;
		{ allocator.Free(ptr) } -> SameAs<void>;
		{ allocator.FreeAll() } -> SameAs<void>;
	};

	/// @brief TODO
	/// @tparam T TODO
	/// @tparam SizeType TODO
	/// @param allocator TODO
	/// @param count TODO
	/// @param alignment TODO
	template <typename T, typename SizeType = USize>
	[[nodiscard]] ORION_FORCE_INLINE constexpr T* AllocateCount(AllocatorKind auto& allocator,
	                                                            SizeType count,
	                                                            SizeType alignment = alignof(T)) noexcept
	{
		return static_cast<T*>(allocator.Allocate(sizeof(T) * count, alignment));
	}
}  // namespace Orion::Engine::Memory
