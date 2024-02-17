#pragma once

#ifdef ORION_GRAPHICS_API_OPENGL

#include "platform/graphics/gpu_atomic_types.h"

#include "glad/glad.h"

namespace orion
{
	/* Returns corresponding OpenGL's type. */
	GLenum to_native_type(gpu_atomic_type type);

	/* Returns corresponding OpenGL's type size*/
	GLsizei to_native_size(gpu_atomic_type type);
}

#endif // ORION_GRAPHICS_API_OPENGL
