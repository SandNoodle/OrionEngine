#if defined(ORION_PLATFORM_LINUX)
#include "Platform/Platform.h"

#include <sys/mman.h>

namespace Orion::Engine::Platform
{
	PlatformInfo GetPlatformInfo() noexcept
	{
		return (PlatformInfo){
			.logical_processor_count = static_cast<UInt32>(get_nprocs()),
			.page_size               = static_cast<UInt64>(getpagesize()),
			.large_page_size         = ORION_MEGABYTES(2),
		};
	}
}  // namespace Orion::Engine::Platform

#endif  // ORION_PLATFORM_LINUX
