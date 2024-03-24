#pragma once

#include "platform/types.h"

#ifdef ORION_GRAPHICS_API_OPENGL
#include "glad/glad.h"
#endif // ORION_GRAPHICS_API_OPENGL

namespace orion
{
	/* Specifies type of buffer requested. */
	enum buffer_type : u8
	{
		buffer_type_vbo,  // Vertex Buffer Object
		buffer_type_ebo,  // Element Buffer Object
		buffer_type_ubo,  // Uniform Buffer Object
		buffer_type_ssbo, // Shader Storage Buffer Object
	};

	/**
	 * @brief Specifies usage of buffer requested.
	 *
	 * @important Explanation of the given modifiers:
	 *  * static  - data is modified once and used many times.
	 *  * dynamic - data is modified repeatedly and used many times.
	 *  * stream  - data is modified once and used at most few times.
	 *  * draw    - data will only be written, not read.
	 *  * read    - data will only be read, not written.
	 *  * copy    - data will be neigther written or read, only copied.
	 */
	enum buffer_usage : u8
	{
		buffer_usage_static_draw,
		buffer_usage_static_read,
		buffer_usage_static_copy,
		buffer_usage_dynamic_draw,
		buffer_usage_dynamic_read,
		buffer_usage_dynamic_copy,
		buffer_usage_stream_draw,
		buffer_usage_stream_read,
		buffer_usage_stream_copy,
	};

	/* Descriptor of a Buffer. */
	struct buffer_desc_t
	{
		buffer_type  type;
		buffer_usage usage;
	};

	/**
	 * @brief Represents given graphics API's underlying buffer object.
	 */
	struct buffer_t
	{
	#ifdef ORION_GRAPHICS_API_OPENGL
		using id_t    = GLuint;
		using type_t  = GLenum;
		using usage_t = GLenum;
	#else
		#error "Unknown Graphics API!"
	#endif // ORION_GRAPHICS_API_OPENGL

		id_t    id;
		type_t  type;
		usage_t usage;
		u32     size;
		u32     element_count;

		/**
		 * @brief Creates new buffer from given descriptor and data on the GPU side.
		 * @param desc buffer descriptor.
		 * @param data pointer to the data that should be passed into GPU side.
		 * @param size total size of the data in bytes.
		 * @param element_count how many 'elements' are stored in the buffer.
		 */
		static buffer_t create(const buffer_desc_t& desc, void* data, u32 size, u32 element_count = 1);

		/** Destroys given buffer on the GPU side. */
		void destroy() const;

		/** Binds buffer for use on the GPU side. */
		void bind() const;

		/** Unbinds buffer from use on the GPU side. */
		void unbind() const;

		/**
		 * Transfers new data to the GPU.
		 *
		 * @important If the new data at an offset would exceed capacity, then:
		 * 1. New buffer will be allocated.
		 * 2. Old buffer's data from range [0, offset) will be copied.
		 * 3. New data will be written starting at an offset.
		 * Beware, this might be costly!
		 *
		 * @param data data to be transferred to the GPU.
		 * @param size size of the data in bytes.
		 * @param offset offset at which the data will be set.
		 */
		void set_data(const void* data, u32 size, u32 offset = 0);
	};
}
