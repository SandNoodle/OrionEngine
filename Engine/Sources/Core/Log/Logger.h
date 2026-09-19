#pragma once

#include "OrionEngine.h"

namespace Orion::Engine::Logger
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

	// NOLINTBEGIN(readability-identifier-naming)
	/// @brief TODO
	Bool8 LoggerSystem_Initialize() noexcept;

	/// @brief TODO
	Bool8 LoggerSystem_Shutdown() noexcept;
	// NOLINTEND(readability-identifier-naming)

	/// @brief TODO
	void LogMessage(Severity severity,
	                CString file,
	                CString function,
	                Int32 line,
	                CString format_message,
	                ...) noexcept;
}  // namespace Orion::Engine::Logger

// --------------------------------------------------------------------------------
// Logger Macros
// --------------------------------------------------------------------------------

#if !defined(ORION_LOGGING_DISABLE)
#define ORION_LOG_FATAL(message, ...)  \
	Orion::Engine::Logger::LogMessage( \
		Orion::Engine::Logger::Severity::Fatal, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define ORION_LOG_ERROR(message, ...)  \
	Orion::Engine::Logger::LogMessage( \
		Orion::Engine::Logger::Severity::Error, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define ORION_LOG_WARN(message, ...)   \
	Orion::Engine::Logger::LogMessage( \
		Orion::Engine::Logger::Severity::Warn, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define ORION_LOG_INFO(message, ...)   \
	Orion::Engine::Logger::LogMessage( \
		Orion::Engine::Logger::Severity::Info, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define ORION_LOG_DEBUG(message, ...)  \
	Orion::Engine::Logger::LogMessage( \
		Orion::Engine::Logger::Severity::Debug, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define ORION_LOG_TRACE(message, ...)  \
	Orion::Engine::Logger::LogMessage( \
		Orion::Engine::Logger::Severity::Trace, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#else
#define ORION_LOG_FATAL(message, ...)
#define ORION_LOG_ERROR(message, ...)
#define ORION_LOG_WARN(message, ...)
#define ORION_LOG_INFO(message, ...)
#define ORION_LOG_DEBUG(message, ...)
#define ORION_LOG_TRACE(message, ...)
#endif
