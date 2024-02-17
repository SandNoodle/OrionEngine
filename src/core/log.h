#pragma once

#include "platform/types.h"
#include "core/builds.h"

namespace orion
{
	enum class log_level : u8
	{
		fatal = 0,
		error = 1,
		warn  = 2,
		info  = 3,
		debug = 4,
		trace = 5,
	};

	void _log_message(log_level level, const char* message, ...);

#ifdef OE_ENABLE_LOGGING
	/**
	 * @brief Should be used for the most fine-grained verbose information,
	 * when full visibility of what is happening is needed.
	 *
	 * @example OE_LOG_TRACE("Created framebuffer: '{}'", framebuffer->to_string());
	 */
	#ifdef OE_ENABLE_LOG_TRACE
		#define OE_LOG_TRACE(message, ...) _log_message(log_level::trace, message, ##__VA_ARGS__);
	#else
		#define OE_LOG_TRACE(message, ...)
	#endif // OE_ENABLE_LOG_TRACE

	/**
	 * @brief Should be used for information that is needed for debugging
	 * purposes.
	 *
	 * @example OE_LOG_DEBUG("Resized a window from {}x{} to {}x{}.",
	 *                       old_width, old_height, new_width, new_height);
	 */
	#ifdef OE_ENABLE_LOG_DEBUG
		#define OE_LOG_DEBUG(message, ...) _log_message(log_level::debug, message, ##__VA_ARGS__);
	#else
		#define OE_LOG_DEBUG(message, ...)
	#endif // OE_ENABLE_LOG_DEBUG

	/**
	 * @brief Should be used for indicating that something expected happened.
	 *
	 * @example OE_LOG_INFO("Loaded a level: '{}'", level->to_string());
	 */
	#ifdef OE_ENABLE_LOG_INFO
		#define OE_LOG_INFO(message, ...) _log_message(log_level::info, message, ##__VA_ARGS__);
	#else
		#define OE_LOG_INFO(message, ...)
	#endif // OE_ENABLE_LOG_INFO

	/**
	 * @brief Should be used for indicating that something unexpected happened,
	 * that might disrupt one of the processes, but where system as a whole
	 * will continue to work.
	 *
	 * @example OE_LOG_WARN("Failed to load a texture: '{}' - returning default.",
	 *                      requestd_texture->to_string());
	 */
	#ifdef OE_ENABLE_LOG_WARN
		#define OE_LOG_WARN(message, ...) _log_message(log_level::warn, message, ##__VA_ARGS__);
	#else
		#define OE_LOG_WARN(message, ...)
	#endif // OE_ENABLE_LOG_WARN

	/**
	 * @brief Should be used when system hits an issue which prevents one or more
	 * functionalities from properly functioning, but where recovery might be possible.
	 *
	 * @example OE_LOG_ERROR("Failed to read a VFS entry: '{}'",
	 *                       requested_entry->to_string());
	 */
	#ifdef OE_ENABLE_LOG_ERROR
		#define OE_LOG_ERROR(message, ...) _log_message(log_level::error, message, ##__VA_ARGS__);
	#else
		#define OE_LOG_ERROR(message, ...)
	#endif // OE_ENABLE_LOG_ERROR

	/**
	 * @brief Should be used when system hits an issue which results in a functionality
	 * not working correctly, where recovery is not possible.
	 *
	 * @example OE_LOG_FATAL("Failed to allocate {} bytes - out of memory!", size);
	 */
	#ifdef OE_ENABLE_LOG_FATAL
		#define OE_LOG_FATAL(message, ...) _log_message(log_level::fatal, message, ##__VA_ARGS__);
	#else
		#define OE_LOG_FATAL(message, ...)
	#endif // OE_ENABLE_LOG_FATAL
#else
	#define OE_LOG_TRACE(message, ...)
	#define OE_LOG_DEBUG(message, ...)
	#define OE_LOG_INFO(message, ...)
	#define OE_LOG_WARN(message, ...)
	#define OE_LOG_ERROR(message, ...)
	#define OE_LOG_FATAL(message, ...)
#endif
}
