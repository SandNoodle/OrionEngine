#if defined(ORION_PLATFORM_WINDOWS)
#include "Platform/Platform.h"

#include <memoryapi.h>
#include <sysinfoapi.h>

namespace Orion::Engine::Platform
{
	PlatformInfo GetPlatformInfo() noexcept
	{
		// System capabilities.
		SYSTEM_INFO system_info;
		GetSystemInfo(&system_info);

		// TODO(SandNoodle): We need a reliable way to get the system name (right now its HARDCODED).
		return (PlatformInfo){
			.system_name             = ORION_STRINGVIEW("Windows"),
			.page_size               = static_cast<UInt64>(system_info.dwPageSize),
			.large_page_size         = static_cast<UInt64>(GetLargePageMinimum()),
			.logical_processor_count = static_cast<UInt32>(system_info.dwNumberOfProcessors),
		};
	}
}  // namespace Orion::Engine::Platform

#endif  // ORION_PLATFORM_WINDOWS
