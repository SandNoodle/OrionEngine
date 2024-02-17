#include "platform/graphics/texture.h"

#ifdef ORION_GRAPHICS_API_OPENGL

#include "core/assert.h"
#include "core/math/omath.h"

namespace orion
{
	static texture_t::format_t get_format(texture_format_t format);
	static texture_t::format_t get_base_format(texture_format_t format);

	texture_t texture_t::create_texture_2d(const texture_desc_t& desc, const void* data, u32 size)
	{
		texture_t t;
		t.type        = GL_TEXTURE_2D;
		t.format      = get_format(desc.format);
		t.base_format = get_base_format(desc.format);
		t.width       = desc.width;
		t.height      = desc.height;
		
		glCreateTextures(t.type, 1, &t.id);
		glTextureParameteri(t.id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(t.id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(t.id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(t.id, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTextureParameteri(t.id, GL_TEXTURE_WRAP_T, GL_REPEAT);

		const b8 generate_mipmaps = desc.flags & texture_flags_generate_mipmaps;
		const usize mipmap_count = math::floor(math::log2(math::max(desc.width, desc.height))) + 1;

		glTextureStorage2D(t.id, mipmap_count, t.format, t.width, t.height);
		glTextureSubImage2D(t.id, 0, 0, 0, t.width, t.height, t.base_format, GL_UNSIGNED_BYTE, data);

		if(generate_mipmaps)
			glGenerateTextureMipmap(t.id);

		return t;
	}

	void texture_t::destroy() const
	{
		glDeleteTextures(1, &id);
	}

	void texture_t::bind() const
	{
		glBindTexture(type, id);
	}

	void texture_t::unbind() const
	{
		glBindTexture(type, 0);
	}

	void texture_t::set_data(const void* data, u32 size)
	{
		OE_TODO("not implemented yet.");
	}

	u8 texture_t::channel_count() const
	{
		switch(format)
		{
			case texture_format_rgb8:
			case texture_format_rgb16:
			case texture_format_rgb32f:
				return 3;
			case texture_format_rgba8:
			case texture_format_rgba16:
			case texture_format_rgba32f:
				return 4;
			default:
				OE_UNREACHABLE();
		}
	}

	//
	//
	//

	static texture_t::format_t get_format(texture_format_t format)
	{
		switch(format)
		{
			case texture_format_rgb8:    return GL_RGB8;
			case texture_format_rgb16:   return GL_RGB16;
			case texture_format_rgb32f:  return GL_RGB32F;
			case texture_format_rgba8:   return GL_RGBA8;
			case texture_format_rgba16:  return GL_RGBA16;
			case texture_format_rgba32f: return GL_RGBA32F;
			default:
				OE_UNREACHABLE("Unhandled texture_format_t case.");
				break;
        }
	}

	static texture_t::format_t get_base_format(texture_format_t format)
	{
		switch(format)
		{
			case texture_format_rgb8:
			case texture_format_rgb16:
			case texture_format_rgb32f:
				return GL_RGB;
			case texture_format_rgba8:
			case texture_format_rgba16:
			case texture_format_rgba32f:
				return GL_RGBA;
			default:
				OE_UNREACHABLE("Unhandled texture_format_t case.");
				break;
        }
	}
}

#endif // ORION_GRAPHICS_API_OPENGL
