#include "platform/console.h"

#ifdef ORION_PLATFORM_LINUX

#include <stdio.h>

namespace orion
{
	void platform_console_write(const char* message, u8 color)
	{
		const char* colour_strings[] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
		printf("\033[%sm%s\033[0m", colour_strings[colour], message);
	}

	void platform_console_write_error(const char* message, u8 color)
	{
		const char* colour_strings[] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
		fprintf(stderr, "\033[%sm%s\033[0m", colour_strings[colour], message);
	}
}

#endif // ORION_PLATFORM_LINUX

