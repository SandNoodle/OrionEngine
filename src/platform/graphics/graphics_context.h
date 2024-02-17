#pragma once

#include "platform/types.h"

#include "platform/graphics/buffer.h"
#include "platform/graphics/shader.h"
#include "platform/graphics/pipeline.h"
#include "platform/graphics/framebuffer.h"

#ifdef ORION_GRAPHICS_API_OPENGL
#include "glad/glad.h"
#else
#error "Unknown Graphics API!"
#endif // ORION_GRAPHICS_API_OPENGL

namespace orion
{
	/** Defines draw command attributes */
	struct draw_attributes_t { };

	/**
	 * Object holding strong references to buffers, shaders, etc.
	 * Performs draw commands.
	 */
	struct graphics_context_t
	{
	#ifdef ORION_GRAPHICS_API_OPENGL
		using id_t    = GLuint;
	#else
		#error "Unknown Graphics API!"
	#endif // ORION_GRAPHICS_API_OPENGL

		id_t           vao;
		u16            vertex_buffers_count;
		buffer_t*      vertex_buffers;
		buffer_t*      index_buffer;
		framebuffer_t* framebuffer;
		pipeline_t*    pipeline;
		shader_t*      shader;

		/** Creates new graphics context from given descriptor and data on the GPU side. */
		static graphics_context_t create(void);

		/** Destroys given graphics context on the GPU side. */
		void destroy() const;

		void draw_indexed(draw_attributes_t attrib) const;
	};
}
