#include "platform/graphics/opengl/opengl_native_types.h"

#ifdef ORION_GRAPHICS_API_OPENGL

#include "core/assert.h"

namespace orion
{
	GLenum to_native_type(gpu_atomic_type type)
	{
		switch(type)
		{
			case  gpu_atomic_type::bool_type:    return GL_BOOL;
			case  gpu_atomic_type::int8_type:    return GL_BYTE;
			case  gpu_atomic_type::int16_type:   return GL_SHORT;
			case  gpu_atomic_type::int32_type:   return GL_INT;
			case  gpu_atomic_type::float16_type: return GL_HALF_FLOAT;
			case  gpu_atomic_type::float32_type: return GL_FLOAT;
			case  gpu_atomic_type::float64_type: return GL_DOUBLE;
			default:
				OE_UNREACHABLE("unhandled gpu_atomic_type case.");
		}
	}

	GLsizei to_native_size(gpu_atomic_type type)
	{
		switch(type)
		{
			case  gpu_atomic_type::bool_type:    return GL_BOOL;
			case  gpu_atomic_type::int8_type:    return GL_BYTE;
			case  gpu_atomic_type::int16_type:   return GL_SHORT;
			case  gpu_atomic_type::int32_type:   return GL_INT;
			case  gpu_atomic_type::float16_type: return GL_HALF_FLOAT;
			case  gpu_atomic_type::float32_type: return GL_FLOAT;
			case  gpu_atomic_type::float64_type: return GL_DOUBLE;
			default:
				OE_UNREACHABLE("unhandled gpu_atomic_type case.");
		}
	}
}

#endif // ORION_GRAPHICS_API_OPENGL
