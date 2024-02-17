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

	// If at least C++11
	#ifndef ORION_CONSTEXPR
		#if __cplusplus > 201103L
			#define ORION_CONSTEXPR constexpr
		#else
			#define ORION_CONSTEXPR
		#endif // C++ version < C++11
	#endif // ORION_CONSTEXP
}
