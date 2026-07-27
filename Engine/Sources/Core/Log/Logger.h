#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Containers/Vector.h"
#include "Core/Standard/Types/StringView.h"

namespace Orion::Engine::Log
{
	/// @brief TODO
	enum class Severity : UInt8
	{
		/// @brief TODO
		Fatal,

		/// @brief TODO
		Error,

		/// @brief TODO
		Warn,

		/// @brief TODO
		Info,

		/// @brief TODO
		Debug,

		/// @brief TODO
		Trace,
	};

	/// @brief TODO
	class ILogger
	{
		public:
		virtual ~ILogger() = default;

		/**
		 * @brief
		 * @param severity
		 * @param formated_message
		 */
		virtual void LogMessage(Severity severity, StringView formated_message) noexcept = 0;
	};

	/// @brief TODO
	class LoggerSystem
	{
		private:
		Vector<ILogger*> _loggers;

		public:
		void Initialize() noexcept;
		void Shutdown() noexcept;
	};

}  // namespace Orion::Engine::Log

// --------------------------------------------------------------------------------
// Logger Macros
// --------------------------------------------------------------------------------

#if !defined(ORION_LOGGING_DISABLE)
#define ORION_LOG_FATAL(message, ...)
// Orion::Engine::Log::LogMessage(   \
// 	Orion::Engine::Log::Severity::Fatal, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define ORION_LOG_ERROR(message, ...)
// Orion::Engine::Log::LogMessage(   \
// 	Orion::Engine::Log::Severity::Error, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define ORION_LOG_WARN(message, ...)
// Orion::Engine::Log::LogMessage(  \
// 	Orion::Engine::Log::Severity::Warn, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define ORION_LOG_INFO(message, ...)
// Orion::Engine::Log::LogMessage(  \
// 	Orion::Engine::Log::Severity::Info, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define ORION_LOG_DEBUG(message, ...)
// Orion::Engine::Log::LogMessage(   \
// 	Orion::Engine::Log::Severity::Debug, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define ORION_LOG_TRACE(message, ...)
// Orion::Engine::Log::LogMessage(   \
// 	Orion::Engine::Log::Severity::Trace, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#else
#define ORION_LOG_FATAL(message, ...)
#define ORION_LOG_ERROR(message, ...)
#define ORION_LOG_WARN(message, ...)
#define ORION_LOG_INFO(message, ...)
#define ORION_LOG_DEBUG(message, ...)
#define ORION_LOG_TRACE(message, ...)
#endif
