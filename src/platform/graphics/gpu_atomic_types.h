#pragma once

#include "platform/types.h"

namespace orion
{
	/* Represents types that can be used on the GPU. */
	enum class gpu_atomic_type : u8
	{
		bool_type,
		int8_type,
		int16_type,
		int32_type,
		float16_type,
		float32_type,
		float64_type,
	};
}
