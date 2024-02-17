#include "log.h"

#include "platform/memory.h"
#include "platform/console.h"

#include <stdio.h>
#include <stdarg.h>

namespace orion
{
	void _log_message(log_level level, const char* message, ...)
	{
		// NOTE: Should match log_level
		const char* prefix[6] = {
			"[FATAL] ",
			"[ERROR] ",
			"[WARN]  ",
			"[INFO]  ",
			"[DEBUG] ",
			"[TRACE] ",
		};

		// NOTE: Limit message to 32k characters.
		char staging[32'000];
		platform_zero_memory(staging, sizeof(staging));

		__builtin_va_list arg_ptr;
		va_start(arg_ptr, message);
		vsnprintf(staging, 32'000, message, arg_ptr);
		va_end(arg_ptr);

		char out_message[32'000];
		sprintf(out_message, "%s%s\n", prefix[(u8)level], staging);

		b8 is_error = level == log_level::fatal || level == log_level::error;
		if(is_error)
			platform_console_write(out_message, (u8)level);
		else
			platform_console_write_error(out_message, (u8)level);

	}
}
