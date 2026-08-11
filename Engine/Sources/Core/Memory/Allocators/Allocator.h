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
}  // namespace Orion::Engine::Memory
