#include "platform/timer.h"

#ifdef ORION_PLATFORM_LINUX

#include <time.h>

namespace orion
{
	f64 platform_get_absolute_time(void)
	{
		struct timespec now;
		clock_gettime(CLOCK_MONOTONIC_RAW, &now);
		return now.tv_sec + now.tv_nsec * 0.000000001;
	}
}

#endif // ORION_PLATFORM_LINUX
