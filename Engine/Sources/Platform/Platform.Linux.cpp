#if defined(ORION_PLATFORM_LINUX)
#include "Platform/Platform.h"

#include <sys/sysinfo.h>
#include <unistd.h>

namespace Orion::Engine::Platform
{
	PlatformInfo GetPlatformInfo() noexcept
	{
		// TODO(SandNoodle): We need a reliable way to get the system name (right now its HARDCODED).
		return (PlatformInfo){
			.system_name             = ORION_STRINGVIEW("Linux"),
			.page_size               = static_cast<UInt64>(getpagesize()),
			.large_page_size         = ORION_MEGABYTES(2),
			.logical_processor_count = static_cast<UInt32>(get_nprocs()),
		};
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

#endif  // ORION_PLATFORM_LINUX
