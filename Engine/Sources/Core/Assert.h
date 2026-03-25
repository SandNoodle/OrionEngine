#pragma once

#include "OrionEngine.h"

#include "Core/Log.h"

/// @brief Evaluates some condition to be true. If the evaluation fails, fatal error is logged.
#define ORION_ASSERT(condition, ...)        \
	do {                                    \
		if (!(condition)) {                 \
			ORION_LOG_FATAL((__VA_ARGS__)); \
			ORION_BUILTIN_TRAP();           \
		}                                   \
	} while (false)

#if defined(ORION_BUILD_DEBUG)
/// @brief Evaluates some condition to be true. If the evaluation fails, fatal error is logged. Check is performed only
/// in Debug builds.
#define ORION_ASSERT_DEBUG(condition, ...)  \
	do {                                    \
		if (!(condition)) {                 \
			ORION_LOG_FATAL((__VA_ARGS__)); \
			ORION_BUILTIN_TRAP();           \
		}                                   \
	} while (false)
#else
#define ORION_ASSERT_DEBUG(condition, ...)
#endif

/// @brief Assertion which is unconditionally triggered after encountering it.
#define ORION_NOT_IMPLEMENTED(...) ORION_ASSERT(false, __VA_ARGS__)
