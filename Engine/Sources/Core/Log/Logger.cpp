#include "Core/Log/Logger.h"

#include "Core/Console/ConsoleSystem.h"

namespace Orion::Engine::Logger
{
	// NOLINTNEXTLINE(readability-identifier-naming)
	static Console::AutoConsoleVariable<Bool8> CVAR_LoggerSystemInitialized(
		"Engine.Log.Initialized",
		false,
		"Returns true/false depending on if the LoggerSystem is initialized (or not).",
		"Engine.Log.Initialized <Bool8>",
		Console::ConsoleVariableFlags::Debug | Console::ConsoleVariableFlags::Read);

	Bool8 LoggerSystem::Initialize() noexcept
	{
		if (CVAR_LoggerSystemInitialized.Value()) {
			ORION_LOG_ERROR("[LoggerSystem] Already initialized.");
			return false;
		}

		CVAR_LoggerSystemInitialized.SetValue(true);

		ORION_LOG_DEBUG("[LoggerSystem] Initialized.");
		return true;
	}

	Bool8 LoggerSystem::Shutdown() noexcept
	{
		if (!CVAR_LoggerSystemInitialized.Value()) {
			ORION_LOG_ERROR("[LoggerSystem] Already shut down.");
			return false;
		}

		CVAR_LoggerSystemInitialized.SetValue(false);

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
