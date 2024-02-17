#include "platform/othread.h"

#ifdef ORION_PLATFORM_WINDOWS

#include "core/log.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace orion
{
	b8 thread_create(thread_t* thread)
	{
		typedef void (*start_fn)();
		start_fn start = nullptr;
		void* params = nullptr;
		thread->internal_data = CreateThread(
		  nullptr,
		  0,
		  (LPTHREAD_START_ROUTINE)start,
		  params,
		  0,
		  (DWORD*)&thread->id);

		if(!thread->internal_data)
			return false;

		OE_LOG_DEBUG("[Thread: %l] started.", thread->id);

		return true;
	}

	void thread_destroy(thread_t* thread)
	{
		if (!thread || !thread->internal_data)
			return;

		DWORD exit_code = 0;
		GetExitCodeThread(thread->internal_data, &exit_code);
		CloseHandle((HANDLE)thread->internal_data);
		thread->internal_data = nullptr;
		thread->id = 0;
	}
}  // namespaceo orion

#endif // ORION_PLATFORM_WINDOWS
