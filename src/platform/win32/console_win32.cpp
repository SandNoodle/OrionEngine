#include "platform/console.h"

#ifdef ORION_PLATFORM_WINDOWS

#include <string.h>

#include <windows.h>

namespace orion
{
	void platform_console_write(const char* message, u8 color)
	{
		HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

		static u8 levels[6] = {64, 4, 6, 2, 1, 8};
		SetConsoleTextAttribute(console_handle, levels[color]);
		OutputDebugStringA(message);
		u64 length = strlen(message);
		DWORD number_written = 0;
		WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, &number_written, 0);

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		SetConsoleTextAttribute(console_handle, csbi.wAttributes);
	}

	void platform_console_write_error(const char* message, u8 color)
	{
		HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);

		static u8 levels[6] = {64, 4, 6, 2, 1, 8};
		SetConsoleTextAttribute(console_handle, levels[color]);
		OutputDebugStringA(message);
		u64 length = strlen(message);
		DWORD number_written = 0;
		WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)length, &number_written, 0);

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		SetConsoleTextAttribute(console_handle, csbi.wAttributes);
	}
}

#endif // ORION_PLATFORM_LINUX
