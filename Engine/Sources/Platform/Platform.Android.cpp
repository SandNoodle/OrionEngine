#if defined(ORION_PLATFORM_ANDROID)

#include "Core/Assert.h"

namespace Orion::Engine::Platform
{
	PlatformInfo GetPlatformInfo() noexcept
	{
		ORION_NOT_IMPLEMENTED();
	}

	Bool8 FileExists(CString path) noexcept
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}

	Bool8 DirectoryExists(CString path) noexcept
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}
}  // namespace Orion::Engine::Platform

#endif
