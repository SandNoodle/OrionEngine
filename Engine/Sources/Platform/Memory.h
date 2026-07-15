#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"

#include <stdlib.h>
#if !defined(ORION_COMPILER_CLANG) && !defined(ORION_COMPILER_GCC)
#include <string.h>
#endif

namespace Orion::Engine::Platform
{
	/**
	 * @brief Allocates \p size_in_bytes sized block of contiguous memory.
	 * @return Returns pointer to the allocated block of memory on success, nullptr otherwise.
	 */
	[[nodiscard]] ORION_FORCE_INLINE constexpr void* MemoryAllocate(USize size_in_bytes) noexcept
	{
		ORION_ASSERT_DEBUG(size_in_bytes > 0, "Cannot perform MemoryAllocate, because size_in_bytes is 0.");
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
		return __builtin_malloc(size_in_bytes);
#else
		return ::malloc(size_in_bytes);
#endif
	}

	/**
	 * @brief Frees previously allocated block of memory.
	 * @warning \p ptr must NOT be null!
	 */
	ORION_FORCE_INLINE constexpr void MemoryFree(void* ptr) noexcept
	{
		ORION_ASSERT_DEBUG(ptr, "Cannot perform MemoryFree, because ptr is null.");
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
		return __builtin_free(ptr);
#else
		return ::free(ptr);
#endif
	}

	/**
	 * @brief Reallocates previously allocated block of memory with new \p size_in_bytes size.
	 * @return Returns pointer to \p size_in_bytes sized reallocated block of memory on success, nullptr otherwise.
	 */
	[[nodiscard]] ORION_FORCE_INLINE constexpr void* MemoryReallocate(void* ptr, USize size_in_bytes) noexcept
	{
		ORION_ASSERT_DEBUG(ptr, "Cannot perform MemoryReallocate, because ptr is null.");
		ORION_ASSERT_DEBUG(size_in_bytes > 0, "Cannot perform MemoryReallocate, because size_in_bytes is 0.");
		return ::realloc(ptr, size_in_bytes);
	}

	/**
	 * @brief Copies \p size_in_bytes sized region of memory from \p src to \p dst.
	 */
	ORION_FORCE_INLINE constexpr void MemoryCopy(void* dst, const void* src, USize size_in_bytes) noexcept
	{
		ORION_ASSERT_DEBUG(dst, "Cannot perform MemoryCopy, because dst is null.");
		ORION_ASSERT_DEBUG(src, "Cannot perform MemoryCopy, because src is null.");
		ORION_ASSERT_DEBUG(size_in_bytes > 0, "Cannot perform MemoryCopy, because size_in_bytes is 0.");
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
		__builtin_memcpy(dst, src, size_in_bytes);
#else
		memcpy(dst, src, size_in_bytes);
#endif
	}

	/**
	 * @brief Moves \p size_in_bytes sized region of memory from \p src to \p dst.
	 */
	ORION_FORCE_INLINE constexpr void MemoryMove(void* dst, const void* src, USize size_in_bytes) noexcept
	{
		ORION_ASSERT_DEBUG(dst, "Cannot perform MemoryMove, because dst is null.");
		ORION_ASSERT_DEBUG(src, "Cannot perform MemoryMove, because src is null.");
		ORION_ASSERT_DEBUG(size_in_bytes > 0, "Cannot perform MemoryMove, because size_in_bytes is 0.");
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
		__builtin_memmove(dst, src, size_in_bytes);
#else
		memmove(dst, src, size_in_bytes);
#endif
	}

	/**
	 * @brief Sets \p size_in_bytes sized region of memory (at \p dst) to a given \p value.
	 */
	ORION_FORCE_INLINE constexpr void MemorySet(void* dst, Int32 value, USize size_in_bytes) noexcept
	{
		ORION_ASSERT_DEBUG(dst, "Cannot perform MemorySet, because dst is null.");
		ORION_ASSERT_DEBUG(size_in_bytes > 0, "Cannot perform MemorySet, because size_in_bytes is 0.");
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
		__builtin_memset(dst, value, size_in_bytes);
#else
		memset(dst, value, size_in_bytes);
#endif
	}

	/**
	 * @brief Sets \p size_in_bytes sized region of memory (at \p dst) to a 0.
	 */
	ORION_FORCE_INLINE constexpr void MemoryZero(void* dst, USize size_in_bytes) noexcept
	{
		ORION_ASSERT_DEBUG(dst, "Cannot perform MemoryZero, because dst is null.");
		ORION_ASSERT_DEBUG(size_in_bytes > 0, "Cannot perform MemoryZero, because size_in_bytes is 0.");
#if defined(ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
		__builtin_memset(dst, 0, size_in_bytes);
#else
		memset(dst, 0, size_in_bytes);
#endif
	}
}  // namespace Orion::Engine::Platform
