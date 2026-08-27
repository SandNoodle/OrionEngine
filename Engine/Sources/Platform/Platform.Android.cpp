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

	Vector<PlatformFileStat> ListFiles(StringView path, Bool8 recursive) noexcept
	{
		ORION_IGNORE_PARAM(path);
		ORION_IGNORE_PARAM(recursive);
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
}  // namespace Orion::Engine::Platform

#endif
