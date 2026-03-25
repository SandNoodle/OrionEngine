#pragma once

#include "OrionEngine.h"

#define ORION_LOG_FATAL(...)
#define ORION_LOG_ERROR(...)
#define ORION_LOG_WARNING(...)
#define ORION_LOG_INFO(...)
#define ORION_LOG_DEBUG(...)
#define ORION_LOG_TRACE(...)

namespace Orion::Engine::Log
{
	/// @brief Initializes logging system for the engine, i.e. enables the use of logging macros.
	void Initialize() noexcept;

	/// @brief Shutdowns the logging system - cleaning up any initialized loggers.
	void Shutdown() noexcept;
}  // namespace Orion::Engine::Log
