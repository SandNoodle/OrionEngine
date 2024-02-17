#pragma once

// ----- [ Windows ] ------
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
	#ifndef _WIN64
		#error "Windows 32-bit is not supported!"
	#endif
	#define ORION_PLATFORM_WINDOWS
// ----- [ Linux ] -----
#elif defined(__linux__) || defined(__gnu_linux__)
	#define ORION_PLATFORM_LINUX
// ----- [ Unix ] -----
#elif defined(__UNIX__)
	#define ORION_PLATFORM_UNIX
// ----- [ Mac ] -----
#elif defined(__APPLE__)
	#error "Apple is not supported! (also lmao)"
#else
	#error "Unknown platform!"
#endif
