#pragma once

#include "platform/types.h"

#include "platform/graphics/buffer.h"
#include "platform/graphics/framebuffer.h"
#include "platform/graphics/pipeline.h"
#include "platform/graphics/shader.h"
#include "platform/graphics/texture.h"

#ifdef ORION_GRAPHICS_API_OPENGL
#include "glad/glad.h"
#else
#error "Unknown Graphics API!"
#endif // ORION_GRAPHICS_API_OPENGL

#include <initializer_list>

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

#if 0
	// @TODO: Rename to graphics_context_t and remove the former after this is done.
	struct render_context_t
	{
		void bind_framebuffer(const framebuffer_t& framebuffer);

		void bind_texture(const texture_t& texture);
		void bind_textures(std::initializer_list<texture_t> texture);

		void bind_buffer(const buffer_t& buffer);
		void bind_buffers(std::initializer_list<buffer_t> buffers);

	};
#endif
}
