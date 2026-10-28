#pragma once

// --- Engine ---
#define ORION_ENGINE_NAME "Orion Engine"
#define ORION_ENGINE_VERSION

// --- Memory System ---
/// @brief Minimal alignment size of a single allocation.
#ifndef ORION_MEMORY_DEFAULT_ALIGNMENT
#define ORION_MEMORY_DEFAULT_ALIGNMENT (2 * sizeof(void*))
#endif  // ORION_MEMORY_DEFAULT_ALIGNMENT

/// @brief Default allocation size for an ArenaAllocator's block.
#ifndef ORION_MEMORY_ALLOCATORS_ARENA_ALLOCATOR_DEFAULT_BLOCK_SIZE
#define ORION_MEMORY_ALLOCATORS_ARENA_ALLOCATOR_DEFAULT_BLOCK_SIZE ORION_KILOBYTES(1)
#endif  // ORION_MEMORY_ALLOCATORS_ARENA_ALLOCATOR_DEFAULT_BLOCK_SIZE

// --- Logging System ---
/// @brief Maximum amount of log files -
#ifndef ORION_LOGGING_MAX_LOG_FILES
#define ORION_LOGGING_MAX_LOG_FILES 5
#endif  // ORION_LOGGING_MAX_LOG_FILES
