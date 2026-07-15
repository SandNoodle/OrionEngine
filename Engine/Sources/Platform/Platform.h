#pragma once

#include "OrionEngine.h"

namespace Orion::Engine::Platform
{
	/// @brief TODO
	struct PlatformInfo
	{
		UInt32 logical_processor_count;
		UInt64 page_size;
		UInt64 large_page_size;
	};

	/// @brief TODO
	[[nodiscard]] PlatformInfo GetPlatformInfo() noexcept;
}  // namespace Orion::Engine::Platform
