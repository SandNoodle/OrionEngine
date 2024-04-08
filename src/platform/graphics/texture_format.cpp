#include "platform/graphics/texture_format.h"

#include "core/assert.h"

namespace orion
{
	u8 texture_format_channel_count(texture_format format)
	{
		switch (format)
		{
			case texture_format::rgb8:
			case texture_format::rgb16:
			case texture_format::rgb32f:
				return 3;
			case texture_format::rgba8:
			case texture_format::rgba16:
			case texture_format::rgba32f:
			case texture_format::depth24stencil8:
				return 4;
			default:
				return 0;
		}
	}

#ifdef ORION_GRAPHICS_API_OPENGL
	GLenum get_format(texture_format format)
	{
		switch(format)
		{
			case texture_format::rgb8:    return GL_RGB8;
			case texture_format::rgb16:   return GL_RGB16;
			case texture_format::rgb32f:  return GL_RGB32F;
			case texture_format::rgba8:   return GL_RGBA8;
			case texture_format::rgba16:  return GL_RGBA16;
			case texture_format::rgba32f: return GL_RGBA32F;
			case texture_format::depth24stencil8: return GL_DEPTH24_STENCIL8;
			default:
				return GL_NONE;
		}
	}
	GLenum get_base_format(texture_format format)
	{
		switch(format)
		{
			case texture_format::rgb8:
			case texture_format::rgb16:
			case texture_format::rgb32f:
				return GL_RGB;
			case texture_format::rgba8:
			case texture_format::rgba16:
			case texture_format::rgba32f:
				return GL_RGBA;
			case texture_format::depth24stencil8:
				return GL_DEPTH_STENCIL;
			default:
				return GL_NONE;
		}
	}
#endif // ORION_GRAPHICS_API_OPENGL
}
