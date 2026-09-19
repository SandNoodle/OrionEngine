#include "Core/Log/Logger.h"

#include "Core/Assert.h"
#include "Core/Standard/Containers/Array.h"
#include "Core/Standard/Containers/StringView.h"
#include "Core/Standard/TypeTraits.h"

namespace Orion::Engine::Logger
{
	ORION_FORCE_INLINE static constexpr StringView SeverityName(Severity level) noexcept
	{
		// NOTE: Must match the order of Severity.
		static constexpr Array k_severity_name = { "Fatal", "Error", "Warn", "Info", "Debug", "Trace" };
		return StringView(k_severity_name[ToUnderlyingType(level)]);
	}

	Bool8 LoggerSystem_Initialize() noexcept
	{
		return true;
	}

	Bool8 LoggerSystem_Shutdown() noexcept
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
