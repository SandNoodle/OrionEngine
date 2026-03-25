#pragma once

// --- Engine ---
#define ORION_ENGINE_NAME "Orion Engine"

// --- Memory System ---
/// @brief Minimal alignment size of a single allocation.
#ifndef ORION_DEFAULT_ALIGNMENT
#define ORION_DEFAULT_ALIGNMENT (2 * sizeof(void*))
#endif  // ORION_DEFAULT_ALIGNMENT

/// @brief Enable tracking of
#ifndef ORION_MEMORY_SYSTEM_ENABLE_STATISTICS
#define ORION_MEMORY_SYSTEM_ENABLE_STATISTICS 1
#endif  // ORION_MEMORY_SYSTEM_ENABLE_STATISTICS
