#pragma once

#include "platform/types.h"

#ifdef ORION_GRAPHICS_API_OPENGL
#include "glad/glad.h"
#endif // ORION_GRAPHICS_API_OPENGL

namespace orion
{
	/** @brief @TODO */
	enum class texture_format : u8
	{
		none,

		rgb8,
		rgb16,
		rgb32f,

		rgba8,
		rgba16,
		rgba32f,

		depth24stencil8,
	};

	/** @brief @TODO */
	enum texture_flags : u8
	{
		none             = 0 << 0,
		generate_mipmaps = 0 << 1,
	};

	/**
	 * @brief @TODO
	 *
	 * @param format
	 * @return
	 */
	u8 texture_format_channel_count(texture_format format);

#ifdef ORION_GRAPHICS_API_OPENGL
	GLenum get_format(texture_format format);
	GLenum get_base_format(texture_format format);
#endif // ORION_GRAPHICS_API_OPENGL
}
