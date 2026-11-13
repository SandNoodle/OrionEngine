#pragma once

// --- Engine ---
#define ORION_ENGINE_NAME "Orion Engine"

// --- Platform ---
/// @brief Size (in bytes) of a single cache line.
#ifndef ORION_PLATFORM_CACHE_SIZE
#define ORION_PLATFORM_CACHE_SIZE ORION_BYTES(64)
#endif  // ORION_MEMORY_CACHE_SIZE

// --- Memory ---
/// @brief Minimal alignment size of a single allocation.
#ifndef ORION_MEMORY_DEFAULT_ALIGNMENT
#define ORION_MEMORY_DEFAULT_ALIGNMENT (2UL * sizeof(void*))
#endif  // ORION_MEMORY_DEFAULT_ALIGNMENT

// --- Core/Standard/Memory ---
/// @brief Default allocation size for an ArenaAllocator's block.
#ifndef ORION_MEMORY_ALLOCATORS_ARENA_ALLOCATOR_DEFAULT_BLOCK_SIZE
#define ORION_MEMORY_ALLOCATORS_ARENA_ALLOCATOR_DEFAULT_BLOCK_SIZE ORION_KILOBYTES(16)
#endif  // ORION_MEMORY_ALLOCATORS_ARENA_ALLOCATOR_DEFAULT_BLOCK_SIZE

// --- Core/Standard/Containers ---
/// @brief Default initial capacity to be allocated when constructing Vector types.
#ifndef ORION_CONTAINERS_VECTOR_INITIAL_CAPACITY
#define ORION_CONTAINERS_VECTOR_INITIAL_CAPACITY 8
#endif ORION_CONTAINERS_VECTOR_INITIAL_CAPACITY

/// @brief Default (storage) size for Function's inline storage.
#ifndef ORION_CONTAINERS_FUNCTION_DEFAULT_STORAGE_SIZE
#define ORION_CONTAINERS_FUNCTION_DEFAULT_STORAGE_SIZE (ORION_PLATFORM_CACHE_SIZE)
#endif  // ORION_CONTAINERS_FUNCTION_DEFAULT_STORAGE_SIZE
