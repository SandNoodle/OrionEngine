#include "platform/graphics/buffer.h"

#ifdef ORION_GRAPHICS_API_OPENGL

#include "core/assert.h"

namespace orion
{
	static GLenum get_type(buffer_type type);
	static GLenum get_usage(buffer_usage usage);

	buffer_t buffer_t::create(const buffer_desc_t& desc, void* data, u32 size, u32 element_count)
	{
		buffer_t b;
		b.usage         = get_usage(desc.usage);
		b.type          = get_type(desc.type);
		b.size          = size;
		b.element_count = element_count;
		glCreateBuffers(1, &b.id);
		glNamedBufferData(b.id, (GLsizeiptr)size, data, b.usage);
		return b;
	}

	void buffer_t::destroy() const
	{
		glDeleteBuffers(1, &id);
	}

	void buffer_t::bind() const
	{
		glBindBuffer(type, id);
	}

	void buffer_t::unbind() const
	{
		glBindBuffer(type, 0);
	}

	void buffer_t::set_data(const void* data, u32 size, u32 offset)
	{
		// Buffer on the GPU side is too small - reallocate.
		if(offset + size > this->size)
		{
			OE_TODO("Buffer reallocation is unimplemented.");
			return;
		}
		glNamedBufferSubData(id, offset, size, data);
	}

	//
	//
	//

	static GLenum get_type(buffer_type type)
	{
		switch(type)
		{
			case buffer_type::vbo:  return GL_ARRAY_BUFFER;
			case buffer_type::ebo:  return GL_ELEMENT_ARRAY_BUFFER;
			case buffer_type::ubo:  return GL_UNIFORM_BUFFER;
			case buffer_type::ssbo: return GL_SHADER_STORAGE_BUFFER;
			default:
				OE_UNREACHABLE();
		}
	}

	static GLenum get_usage(buffer_usage usage)
	{
		switch(usage)
		{
			case buffer_usage::static_draw:  return GL_STATIC_DRAW;
			case buffer_usage::static_read:  return GL_STATIC_READ;
			case buffer_usage::static_copy:  return GL_STATIC_COPY;
			case buffer_usage::dynamic_draw: return GL_DYNAMIC_DRAW;
			case buffer_usage::dynamic_read: return GL_DYNAMIC_READ;
			case buffer_usage::dynamic_copy: return GL_DYNAMIC_COPY;
			case buffer_usage::stream_draw:  return GL_STREAM_DRAW;
			case buffer_usage::stream_read:  return GL_STREAM_READ;
			case buffer_usage::stream_copy:  return GL_STREAM_COPY;
			default:
				OE_UNREACHABLE();
		}
	}
}

#endif // ORION_GRAPHICS_API_OPENGL
