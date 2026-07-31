#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Containers/StringView.h"

namespace Orion::Engine::Platform
{
	/// @brief Structure holding capabilities of the underlying platform.
	struct PlatformInfo
	{
		/// @brief Name of the underlying operating system, ex. Windows, Linux.
		StringView system_name;

		/// @brief Allocation size of a single memory page.
		UInt64 page_size;

		/// @brief Allocation size of a single large memory page.
		UInt64 large_page_size;

		/// @brief Number of logical processors that are available to the engine.
		UInt32 logical_processor_count;
	};

	/// @brief Queries and returns capabilities of the underlying platform.
	[[nodiscard]] PlatformInfo GetPlatformInfo() noexcept;
}  // namespace Orion::Engine::Platform
