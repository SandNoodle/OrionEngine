#include "platform/timer.h"

#ifdef ORION_PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace orion
{
	static f64 g_clock_frequency;
	static LARGE_INTEGER g_start_time;
	static void setup_clock()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		g_clock_frequency = 1.0 / (f64)frequency.QuadPart;
		QueryPerformanceCounter(&g_start_time);
	}

	f64 platform_get_absolute_time(void)
	{
		if(!g_clock_frequency)
			setup_clock();

		LARGE_INTEGER now_time;
		QueryPerformanceCounter(&now_time);
		return (f64)now_time.QuadPart * g_clock_frequency;
	}
}

#endif // ORION_PLATFORM_WINDOWS
