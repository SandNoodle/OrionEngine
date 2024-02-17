#include "platform/memory.h"

#ifdef ORION_PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace orion
{
	void* platform_allocate(size_t n)
	{
		return (void*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, n);
	}

	void* platform_reallocate(void* block, size_t new_size)
	{
		return (void*)HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, block, new_size);
	}

	void platform_free(void* block)
	{
		HeapFree(GetProcessHeap(), 0, block);
	}

	void* platform_zero_memory(void* block, size_t size)
	{
		return (void*)ZeroMemory(block, size);
	}

	void* platform_copy_memory(void* dst, const void* src, size_t size)
	{
		return (void*)CopyMemory(dst, src, size);
	}

	void* platform_set_memory(void* dst, i32 value, size_t size)
	{
		return (void*)FillMemory(dst, value, size);
	}
}

#endif // ORION_PLATFORM_WINDOWS
