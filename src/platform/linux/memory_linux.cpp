#include "platform/memory.h"

#ifdef ORION_PLATFORM_LINUX

#include <stdlib.h>

namespace orion
{
	void* platform_allocate(size_t n)
	{
		return (void*)malloc(n);
	}

	void* platform_reallocate(void* block, size_t new_size)
	{
		return (void*)realloc(block, size);
	}

	void platform_free(void* block)
	{
		free(block);
	}

	void* platform_zero_memory(void* block, size_t size)
	{
		return (void*)memset(block, 0, size);
	}

	void* platform_copy_memory(void* dst, const void* src, size_t size)
	{
		return (void*)memcpy(dst, src, size);
	}

	void* platform_set_memory(void* dst, i32 value, size_t size)
	{
		return (void*)memset(dst, value, size);
	}
}

#endif // ORION_PLATFORM_LINUX
