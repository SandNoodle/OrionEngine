#pragma once

#include "platform/platform.h"
#include "platform/types.h"

namespace orion
{
	/**
	 * @brief Sends message to platform stdout's channel.
	 *
	 * @param message message to be sent.
	 * @param color color of the message.
	 */
	void platform_console_write(const char* message, u8 color);

	/**
	 * @brief Sends message to platform stderr's channel.
	 *
	 * @param message message to be sent.
	 * @param color color of the message.
	 */
	void platform_console_write_error(const char* message, u8 color);
}
