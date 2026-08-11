#pragma once

#include "OrionEngine.h"

/// @brief Evaluates some condition to be true. If the evaluation fails, fatal error is logged.
#define ORION_ASSERT(condition, ...) \
	do {                             \
		if (!(condition)) {          \
			ORION_BUILTIN_TRAP();    \
		}                            \
	} while (false)

#if defined(ORION_BUILD_DEBUG)
/// @brief Evaluates some condition to be true. If the evaluation fails, fatal error is logged. Check is performed only
/// in Debug builds.
#define ORION_ASSERT_DEBUG(condition, ...) ORION_ASSERT(condition, ##__VA_ARGS__)
#else
#define ORION_ASSERT_DEBUG(condition, ...)
#endif

#if defined(ORION_BUILD_DEBUG) && defined(ORION_ENGINE_ENABLE_SLOW_ASSERTS)
/// @brief Evaluates some condition to be true. If the evaluation fails, fatal error is logged. Check is performed only
/// in Debug builds (with slow checks enabled).
#define ORION_ASSERT_DEBUG_SLOW(condition, ...) ORION_ASSERT(condition, ##__VA_ARGS__)
#else
#define ORION_ASSERT_DEBUG_SLOW(condition, ...)
#endif

/// @brief Assertion which is unconditionally triggered after encountering it.
#define ORION_NOT_IMPLEMENTED(...) ORION_ASSERT(false, ##__VA_ARGS__)
