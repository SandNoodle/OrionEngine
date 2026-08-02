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
		return ORION_STRINGVIEW(k_severity_name[ToUnderlyingType(level)]);
	}

	LoggerSystem& LoggerSystem::Get() noexcept
	{
		static LoggerSystem logger;
		return logger;
	}

	void LoggerSystem::Initialize() noexcept
	{
	}

	void LoggerSystem::Shutdown() noexcept
	{
	}
}  // namespace Orion::Engine::Log
