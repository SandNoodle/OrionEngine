#pragma once

#include "platform.h"
#include "platform/types.h"

namespace orion
{
	/** Returns the absolute time since the application started. */
	f64 platform_get_absolute_time(void);
}
