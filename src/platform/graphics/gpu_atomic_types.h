#pragma once

#include "platform/types.h"

namespace orion
{
	/* Represents types that can be used on the GPU. */
	enum class gpu_atomic_type : u8
	{
		type_bool,
		type_int8,
		type_int16,
		type_int32,
		type_float16,
		type_float32,
		type_float64,
	};
}
