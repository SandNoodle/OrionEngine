#include "platform/graphics/framebuffer.h"

#ifdef ORION_GRAPHICS_API_OPENGL

#include "platform/memory.h"

#include "core/assert.h"

namespace orion
{
	static GLenum get_format(framebuffer_color_texture_format_t format);
	static GLenum get_base_format(framebuffer_color_texture_format_t format);
	static GLenum get_format(framebuffer_depth_texture_format_t format);

	framebuffer_t framebuffer_t::create(const framebuffer_desc_t& desc)
	{
		framebuffer_t f;
		platform_copy_memory(
			f.color_textures_formats,
			desc.color_texture_formats,
			desc.color_texture_formats_count);
		f.depth_texture_format   = desc.depth_texture_format;
		f.color_textures_count   = desc.color_texture_formats_count;
		f.width                  = desc.width;
		f.height                 = desc.height;
		f.recreate(desc.width, desc.height);
		return f;
	}

	void framebuffer_t::destroy() const
	{
		glDeleteTextures(1, &depth_texture_id);
		glDeleteTextures(color_textures_count, color_textures_ids);
		glDeleteFramebuffers(1, &id);
	}

	void framebuffer_t::bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);
	}

	void framebuffer_t::unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	}

	void framebuffer_t::clear_color_texture(usize index, const float* color) const
	{
		if(id == GL_NONE)
		{
			glClearNamedFramebufferfv(id, GL_COLOR, 0, (const GLfloat*)color);
			return;
		}

		const id_t texture_index = color_textures_ids[index];
		const GLenum base_format = get_base_format(color_textures_formats[index]);
		glClearTexImage(texture_index, 0, base_format, GL_FLOAT, (const void*)color);
	}

	void framebuffer_t::clear_color_textures(const float* color) const
	{
		for(usize index = 0; index < color_textures_count; ++index)
			clear_color_texture(index, color);
	}

	void framebuffer_t::clear_depth_texture() const
	{
		glClearNamedFramebufferfi(depth_texture_id, GL_DEPTH_STENCIL, 0, 1.0f, 0);
	}

	void framebuffer_t::resize(u16 width, u16 height)
	{
		b8 status = recreate(width, height);
		if(!status)
		{
			OE_LOG_ERROR("Failed to recreate framebuffer!");
			return;
		}
		this->width  = width;
		this->height = height;
	}

	b8 framebuffer_t::recreate(u16 width, u16 height)
	{
		if(id)
		{
			glDeleteTextures(1, &depth_texture_id);
			glDeleteTextures(color_textures_count, color_textures_ids);
			glDeleteFramebuffers(1, &id);
			platform_zero_memory(color_textures_ids, sizeof(id_t) * ORION_GRAPHICS_MAX_TEXTURE_SAMPLERS);
			depth_texture_id = GL_NONE;
		}

		glCreateFramebuffers(1, &id);

		glCreateTextures(GL_TEXTURE_2D, color_textures_count, color_textures_ids);
		for(usize i = 0; i < color_textures_count; ++i)
		{
			const id_t texture_id = color_textures_ids[i];
			glTextureParameteri(texture_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(texture_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTextureParameteri(texture_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(texture_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTextureParameteri(texture_id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTextureStorage2D(texture_id, 1, get_format(color_textures_formats[i]), width, height);
			glNamedFramebufferTexture(id, GL_COLOR_ATTACHMENT0 + i, texture_id, 0);
		}

		if(depth_texture_format != framebuffer_depth_format_none)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &depth_texture_id);
			glTextureParameteri(depth_texture_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(depth_texture_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTextureParameteri(depth_texture_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTextureParameteri(depth_texture_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTextureParameteri(depth_texture_id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
			glTextureStorage2D(depth_texture_id, 1, get_format(depth_texture_format), width, height);
			glNamedFramebufferTexture(id, GL_DEPTH_STENCIL_ATTACHMENT, depth_texture_id, 0);
		}

		const GLenum buffers[ORION_GRAPHICS_MAX_TEXTURE_SAMPLERS] = {
			GL_COLOR_ATTACHMENT0,  GL_COLOR_ATTACHMENT1,  GL_COLOR_ATTACHMENT2,  GL_COLOR_ATTACHMENT3,
			GL_COLOR_ATTACHMENT4,  GL_COLOR_ATTACHMENT5,  GL_COLOR_ATTACHMENT6,  GL_COLOR_ATTACHMENT7,
			GL_COLOR_ATTACHMENT8,  GL_COLOR_ATTACHMENT9,  GL_COLOR_ATTACHMENT10, GL_COLOR_ATTACHMENT11,
			GL_COLOR_ATTACHMENT12, GL_COLOR_ATTACHMENT13, GL_COLOR_ATTACHMENT14, GL_COLOR_ATTACHMENT15,
		};
		glNamedFramebufferDrawBuffers(id, color_textures_count, buffers);

		GLenum status = glCheckNamedFramebufferStatus(id, GL_FRAMEBUFFER);
		return status == GL_FRAMEBUFFER_COMPLETE;
	}

	//
	//
	//

	static GLenum get_format(framebuffer_color_texture_format_t format)
	{
		switch(format)
		{
			case framebuffer_color_format_rgba8:  return GL_RGBA8;
			case framebuffer_color_format_rgba16: return GL_RGBA16;
			default:
				OE_UNREACHABLE();
		}
	}

	static GLenum get_base_format(framebuffer_color_texture_format_t format)
	{
		switch(format)
		{
			case framebuffer_color_format_rgba8:  return GL_RGBA;
			case framebuffer_color_format_rgba16: return GL_RGBA;
			default:
				OE_UNREACHABLE();
		}
	}

	static GLenum get_format(framebuffer_depth_texture_format_t format)
	{
		switch(format)
		{
			case framebuffer_depth_format_depth24stencil8:
				return GL_DEPTH24_STENCIL8;
			default:
				OE_UNREACHABLE();
		}
	}
}

#endif // ORION_GRAPHICS_API_OPENGL
