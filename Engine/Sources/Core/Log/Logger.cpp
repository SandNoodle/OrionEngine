#include "Core/Log/Logger.h"

namespace Orion::Engine::Logger
{
	Bool8 LoggerSystem::Initialize() noexcept
	{
		ORION_LOG_DEBUG("[LoggerSystem] Initialized.");
		return true;
	}

	Bool8 LoggerSystem::Shutdown() noexcept
	{
		return true;
	}

	void LogMessage(Severity severity, CString file, CString function, Int32 line, CString format_message, ...) noexcept
	{
		ORION_IGNORE_PARAM(severity);
		ORION_IGNORE_PARAM(file);
		ORION_IGNORE_PARAM(function);
		ORION_IGNORE_PARAM(line);
		ORION_IGNORE_PARAM(format_message);
		// TODO(SandNoodle): Format the message and pass it to the LoggerSystem.
	}
}  // namespace Orion::Engine::Logger
