#include "platform/graphics/opengl/opengl_native_types.h"

#ifdef ORION_GRAPHICS_API_OPENGL

#include "core/assert.h"

namespace orion
{
	GLenum to_native_type(gpu_atomic_type type)
	{
		switch(type)
		{
			case  gpu_atomic_type::type_bool:    return GL_BOOL;
			case  gpu_atomic_type::type_int8:    return GL_BYTE;
			case  gpu_atomic_type::type_int16:   return GL_SHORT;
			case  gpu_atomic_type::type_int32:   return GL_INT;
			case  gpu_atomic_type::type_float16: return GL_HALF_FLOAT;
			case  gpu_atomic_type::type_float32: return GL_FLOAT;
			case  gpu_atomic_type::type_float64: return GL_DOUBLE;
			default:
				OE_UNREACHABLE("unhandled gpu_atomic_type case.");
				return GL_NONE;
		}
	}

	GLsizei to_native_size(gpu_atomic_type type)
	{
		switch(type)
		{
			case  gpu_atomic_type::type_bool:    return GL_BOOL;
			case  gpu_atomic_type::type_int8:    return GL_BYTE;
			case  gpu_atomic_type::type_int16:   return GL_SHORT;
			case  gpu_atomic_type::type_int32:   return GL_INT;
			case  gpu_atomic_type::type_float16: return GL_HALF_FLOAT;
			case  gpu_atomic_type::type_float32: return GL_FLOAT;
			case  gpu_atomic_type::type_float64: return GL_DOUBLE;
			default:
				OE_UNREACHABLE("unhandled gpu_atomic_type case.");
				return GL_NONE;
		}
	}
}

#endif // ORION_GRAPHICS_API_OPENGL
