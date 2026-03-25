#pragma once

#include "OrionEngine.h"

namespace Orion::Engine
{
	/// @brief This is the Main Entry point of the engine. It initializes all required modules, before starting the
	/// application.
	void Initialize(int argc, char** argv) noexcept;

	/// @brief Shuts down the engine module - shutdowns all engine systems.
	void Shutdown() noexcept;
}  // namespace Orion::Engine
