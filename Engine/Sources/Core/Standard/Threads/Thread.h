#pragma once

#include "OrionEngine.h"

namespace Orion::Engine
{
	/// @brief TODO
	struct ThreadID
	{
	};

	/// @brief TODO
	class Thread
	{
		public:
		Thread() noexcept;
		Thread(const Thread&) noexcept = delete;
		Thread(Thread&& other) noexcept;
	};
}  // namespace Orion::Engine
