#if defined(ORION_PLATFORM_ANDROID)

#include "Core/Assert.h"

namespace Orion::Engine::Platform
{
	PlatformInfo GetPlatformInfo() noexcept
	{
		ORION_NOT_IMPLEMENTED();
	}

	Bool8 FileCreate(StringView path, PlatformFileAccessFlags flags) noexcept
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}

	Bool8 FileRemove(StringView path) noexcept
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}

	Bool8 FileExists(StringView path) noexcept
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}

	PlatformFileStat StatFile(StringView path) noexcept
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}

	Vector<PlatformFileStat> ListFiles(StringView path, PlatformListOption list_option) noexcept
	{
		ORION_IGNORE_PARAM(path);
		ORION_IGNORE_PARAM(list_option);
		ORION_NOT_IMPLEMENTED();
	}

	Bool8 DirectoryCreate(StringView path) noexcept
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}

	Bool8 DirectoryRemove(StringView path) noexcept
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}

	Bool8 DirectoryExists(StringView path) noexcept
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}
	PlatformThreadHandle ThreadCreate(PlatformThreadFn thread_function, void* user_data) noexcept
	{
		ORION_IGNORE_PARAM(thread_function);
		ORION_IGNORE_PARAM(user_data);
		ORION_NOT_IMPLEMENTED();
	}

	void ThreadDestroy(PlatformThreadHandle thread) noexcept
	{
		ORION_IGNORE_PARAM(thread);
		ORION_NOT_IMPLEMENTED();
	}

	void ThreadJoin(PlatformThreadHandle thread) noexcept
	{
		ORION_IGNORE_PARAM(thread);
		ORION_NOT_IMPLEMENTED();
	}

	void ThreadDetach(PlatformThreadHandle thread) noexcept
	{
		ORION_IGNORE_PARAM(thread);
		ORION_NOT_IMPLEMENTED();
	}
}  // namespace Orion::Engine::Platform

#endif
