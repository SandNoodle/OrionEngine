#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Containers/StringView.h"
#include "Core/Standard/Containers/Vector.h"

namespace Orion::Engine::Log
{
	/// @brief Represents the categorized 'level' of logging.
	enum class Severity : UInt8
	{
		/// @brief Used when the engine encounters an error it cannot possibly recover from, and the shutdown is
		/// imminent. Example: Failure to allocate memory for critical systems.
		Fatal,

		/// @brief Used when engine encounter an error from which recovery might not be possible, and its likely it will
		/// result in a shutdown or cause the application to run improperly. Example: Failing to initialize requested
		/// graphics API.
		Error,

		/// @brief Used to indicate when the engine might run sub-optimally. Example: Trying to load non-existent game
		/// assets.
		Warn,

		/// @brief Used to indicate non-erroneous information. Example: Successfully resizing game's window and logging
		/// its (new) dimensions.
		Info,

		/// @brief Used in development builds for debugging purposes. Example: Logging initialization / shutdown of an
		/// engine's internal sub-system/module.
		Debug,

		/// @brief Used in development builds for debugging purposes, but where the data logged might be too granular
		/// for a typical debug build. Example: Obtaining a lock on a mutex.
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

	constexpr void LogMessage(Severity severity,
	                          CString file,
	                          CString function,
	                          Int32 line,
	                          CString format_message,
	                          ...) noexcept
	{
		ORION_IGNORE_PARAM(severity);
		ORION_IGNORE_PARAM(file);
		ORION_IGNORE_PARAM(function);
		ORION_IGNORE_PARAM(line);
		ORION_IGNORE_PARAM(format_message);
		// TODO(SandNoodle): Format the message and pass it to the LoggerSystem.
	}
}  // namespace Orion::Engine::Log

// --------------------------------------------------------------------------------
// Logger Macros
// --------------------------------------------------------------------------------

#if !defined(ORION_LOGGING_DISABLE)
#define ORION_LOG_FATAL(message, ...) \
	Orion::Engine::Log::LogMessage(   \
		Orion::Engine::Log::Severity::Fatal, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define ORION_LOG_ERROR(message, ...) \
	Orion::Engine::Log::LogMessage(   \
		Orion::Engine::Log::Severity::Error, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define ORION_LOG_WARN(message, ...) \
	Orion::Engine::Log::LogMessage(  \
		Orion::Engine::Log::Severity::Warn, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define ORION_LOG_INFO(message, ...) \
	Orion::Engine::Log::LogMessage(  \
		Orion::Engine::Log::Severity::Info, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define ORION_LOG_DEBUG(message, ...) \
	Orion::Engine::Log::LogMessage(   \
		Orion::Engine::Log::Severity::Debug, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define ORION_LOG_TRACE(message, ...) \
	Orion::Engine::Log::LogMessage(   \
		Orion::Engine::Log::Severity::Trace, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#else
#define ORION_LOG_FATAL(message, ...)
#define ORION_LOG_ERROR(message, ...)
#define ORION_LOG_WARN(message, ...)
#define ORION_LOG_INFO(message, ...)
#define ORION_LOG_DEBUG(message, ...)
#define ORION_LOG_TRACE(message, ...)
#endif
