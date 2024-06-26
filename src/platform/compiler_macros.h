#pragma once

namespace orion
{
	#if defined(__GNUC__) // Gcc, Clangd
		#define OE_BUILTIN_TRAP() __builtin_trap()
		#define OE_BUILTIN_UNREACHABLE() __builtin_unreachable()
		#define OE_FORCE_INLINE __attribute__((always_inline))
	#elif defined(_MSC_VER) // MSVC
		#include <intrin.h>
		#define OE_BUILTIN_TRAP() __debugbreak()
		#define OE_BUILTIN_UNREACHABLE() __assume(false)
		#define OE_FORCE_INLINE __force_inline
	#else
		#define OE_BUILTIN_TRAP()
		#define OE_BUILTIN_UNREACHABLE()
		#define OE_FORCE_INLINE
		#error Unknown compiler detected.
	#endif

	// TODO: force always inline.
	#ifndef ORION_INLINE
	#define ORION_INLINE inline
	#endif // ORION_INLINE

	#ifndef ORION_SUPPRESS_UNUSED
	#define ORION_SUPPRESS_UNUSED(x) (void)(x)
	#endif // ORION_SUPPRESS_UNUSED
}
