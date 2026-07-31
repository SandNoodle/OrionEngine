#include "Core/Log/Logger.h"

#include "Core/Standard/Containers/StringView.h"
#include "Core/Assert.h"
#include "Core/Standard/Containers/Array.h"
#include "Core/Standard/TypeTraits.h"

namespace Orion::Engine::Log
{
	ORION_FORCE_INLINE static constexpr StringView SeverityName(Severity level) noexcept
	{
		// NOTE: Must match the order of Severity.
		static constexpr Array k_severity_name = { "Fatal", "Error", "Warn", "Info", "Debug", "Trace" };
		return ORION_STRINGVIEW(k_severity_name[ToUnderlyingType(level)]);
	}

	class FileLogger : public ILogger
	{
		public:
		~FileLogger() override = default;

		void LogMessage(Severity severity, StringView formated_message) noexcept override;
	};

	void FileLogger::LogMessage(Severity severity, StringView formated_message) noexcept
	{
		ORION_IGNORE_PARAM(formated_message);

		StringView severity_name = SeverityName(severity);
		ORION_IGNORE_PARAM(severity_name);
	}
}  // namespace Orion::Engine::Log
