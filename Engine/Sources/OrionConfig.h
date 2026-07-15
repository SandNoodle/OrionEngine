#pragma once

// --- Engine ---
#define ORION_ENGINE_NAME "Orion Engine"

// --- Memory System ---
/// @brief Minimal alignment size of a single allocation.
#ifndef ORION_DEFAULT_ALIGNMENT
#define ORION_DEFAULT_ALIGNMENT (2 * sizeof(void*))
#endif  // ORION_DEFAULT_ALIGNMENT

// --- Logging System ---

/// @brief Maximum amount of log files -
#ifndef ORION_LOGGING_MAX_LOG_FILES
#define ORION_LOGGING_MAX_LOG_FILES 5
#endif  // ORION_LOGGING_MAX_LOG_FILES