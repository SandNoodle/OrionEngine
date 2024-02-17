#pragma once

/**
 * @file assert.h
 *
 * @brief Provides assertion validating macros.
 */

#include "platform/compiler_macros.h"
#include "core/log.h"

namespace orion
{
#ifdef OE_ENABLE_ASSERTIONS
	/**
	 * @brief Asserts that condition is met, otherwise it logs failure.
	 *
	 * @param condition condition expected.
	 * @param ... optional assertion message.
	 */
	#define OE_ASSERT(condition, ...)       \
		{                                   \
			if(!(condition))                \
			{                               \
				OE_LOG_FATAL((__VA_ARGS__)) \
				OE_BUILTIN_TRAP()           \
			}                               \
		}

	#define OE_ASSERT_TRUE(condition, ...) OE_ASSERT((condition), (__VA_ARGS__));
	#define OE_ASSERT_FALSE(condition, ...) OE_ASSERT(!(condition), (__VA_ARGS__));

	/**
	 * @brief Assertion that automatically fails with message when executed.
	 *
	 * @param ... optional message.
	 */
	#define OE_TODO(...) OE_ASSERT(false, (__VA_ARGS__));

#else
	#define OE_ASSERT(condition, ...)
	#define OE_ASSERT_TRUE(condition, ...)
	#define OE_ASSERT_FALSE(condition, ...)
	#define OE_TODO(...)
#endif // OE_ENABLE_ASSERTIONS

	/**
	 * @brief Specifies that execution path is unreachable,
	 * which prompts compiler to optimize.
	 */
	#define OE_UNREACHABLE(...) OE_BUILTIN_UNREACHABLE()
}
