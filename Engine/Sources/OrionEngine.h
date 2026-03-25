#pragma once

#include "OrionConfig.h"

#include <float.h>
#include <stdint.h>

// ---- [ Platform ] ----
#if defined(ORION_PLATFORM_WINDOWS)

#ifndef _WIN64
#error "Only 64-bit Windows is supported."
#endif

#elif defined(ORION_PLATFORM_LINUX)
#elif defined(ORION_PLATFORM_MAC)
#error "Mac OS is not supported."

#else
#error "Unkown platform detected."
#endif

// ---- [ Compiler detection ] ----
#if defined(__clang__)
#define ORION_COMPILER_CLANG 1

#define ORION_FORCE_INLINE __attribute__((always_inline))
#define ORION_NEVER_INLINE __attribute__((noinline))

#define ORION_BUILTIN_TRAP() __builtin_trap()

#elif defined(__GNUC__) || defined(__GNUG__)
#define ORION_COMPILER_GCC 1

#define ORION_FORCE_INLINE __attribute__((always_inline))
#define ORION_NEVER_INLINE __attribute__((noinline))

#define ORION_BUILTIN_TRAP() __builtin_trap()

#elif defined(_MSC_VER)
#error "MSVC is not supported."

#else
#error "Unknown compiler detected."
#endif

// ---- [ Dynamic Linking ]  ----
// Export
#ifdef ORION_EXPORT
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
#define ORION_API __attribute__((visibility("default")))
#else
#error "Cannot define ORION_API, because compiler is unknown."
#endif

// Import
#else
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
#define ORION_API
#else
#error "Cannot define ORION_API, because compiler is unknown."
#endif
#endif

// ---- [ Units ] ----
#define ORION_GIGABYTES(x) ((x) * 1024ULL * 1024ULL * 1024ULL)
#define ORION_MEGABYTES(x) ((x) * 1024ULL * 1024ULL)
#define ORION_KILOBYTES(x) ((x) * 1024ULL)
#define ORION_BYTES(x) ((x))

// ---- [ Miscellaneous ] ----
#define ORION_MIN(x, y) ((x) < (y) ? (x) : (y))
#define ORION_MAX(x, y) ((x) > (y) ? (x) : (y))
#define ORION_CLAMP(v, min, max) (((v) <= (min)) ? (min) : ((v) >= (max)) ? (max) : (v))

#define ORION_IGNORE_PARAM(x, ...) (void)(x)

#define ORION_EXIT_SUCCESS 0
#define ORION_EXIT_FAILURE 1

// ---- [ Base types ] ----
namespace Orion
{
	using Byte = uint8_t;

	using Bool8  = uint8_t;
	using Bool16 = uint16_t;
	using Bool32 = uint32_t;
	using Bool64 = uint64_t;

	using UInt8  = uint8_t;
	using UInt16 = uint16_t;
	using UInt32 = uint32_t;
	using UInt64 = uint64_t;

	using Int8  = int8_t;
	using Int16 = int16_t;
	using Int32 = int32_t;
	using Int64 = int64_t;

	using Float32 = float;
	using Float64 = double;

	using Char8  = uint8_t;
	using Char16 = uint16_t;
	using Char32 = uint32_t;

	using USize = UInt64;
	using ISize = Int64;

#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
	using UInt128 = __uint128_t;
	using Int128  = __int128_t;

	static_assert(sizeof(UInt128) == 16, "Expected UInt128 to be 16 bytes.");
	static_assert(sizeof(Int128) == 16, "Expected Int128 to be 16 bytes.");
#else
#error "Cannot declare 128 bit integer types."
#endif
	static_assert(sizeof(Bool8) == 1, "Expected Bool8 to be 1 byte.");
	static_assert(sizeof(Bool16) == 2, "Expected Bool16 to be 2 bytes.");
	static_assert(sizeof(Bool32) == 4, "Expected Bool32 to be 4 bytes.");
	static_assert(sizeof(Bool64) == 8, "Expected Bool64 to be 8 bytes.");

	static_assert(sizeof(UInt8) == 1, "Expected UInt8 to be 1 byte.");
	static_assert(sizeof(UInt16) == 2, "Expected UInt16 to be 2 bytes.");
	static_assert(sizeof(UInt32) == 4, "Expected UInt32 to be 4 bytes.");
	static_assert(sizeof(UInt64) == 8, "Expected UInt64 to be 8 bytes.");

	static_assert(sizeof(Int8) == 1, "Expected Int8 to be 1 byte.");
	static_assert(sizeof(Int16) == 2, "Expected Int16 to be 2 bytes.");
	static_assert(sizeof(Int32) == 4, "Expected Int32 to be 4 bytes.");
	static_assert(sizeof(Int64) == 8, "Expected Int64 to be 8 bytes.");

	static_assert(sizeof(Float32) == 4, "Expected Float32 to be 4 bytes.");
	static_assert(sizeof(Float64) == 8, "Expected Float64 to be 8 bytes.");

	static_assert(sizeof(Char8) == 1, "Expected Char8 to be 1 byte.");
	static_assert(sizeof(Char16) == 2, "Expected Char16 to be 2 bytes.");
	static_assert(sizeof(Char32) == 4, "Expected Char32 to be 4 bytes.");
}  // namespace Orion
