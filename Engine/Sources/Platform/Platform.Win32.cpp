#if defined(ORION_PLATFORM_WINDOWS)
#include "Platform/Platform.h"

#include <memoryapi.h>
#include <sysinfoapi.h>

namespace Orion::Engine::Platform
{
	PlatformInfo GetPlatformInfo() noexcept
	{
		SYSTEM_INFO system_info;
		GetSystemInfo(&system_info);

		return (PlatformInfo){
			.logical_processor_count = static_cast<UInt32>(system_info.dwNumberOfProcessors),
			.page_size               = static_cast<UInt64>(system_info.dwPageSize),
			.large_page_size         = static_cast<UInt64>(GetLargePageMinimum()),
		};
	}
}  // namespace Orion::Engine::Platform

#endif  //  ORION_PLATFORM_WINDOWS
