#include "platform/graphics/texture.h"

#ifdef ORION_GRAPHICS_API_OPENGL

#include "core/assert.h"

#include <cmath>

namespace orion
{
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

		const b8 generate_mipmaps = desc.flags & texture_flags::generate_mipmaps;
		const usize mipmap_count = std::floor(std::log2(std::max(desc.width, desc.height))) + 1;

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
}

#endif // ORION_GRAPHICS_API_OPENGL
