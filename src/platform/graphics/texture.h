#pragma once

#include "platform/types.h"

#ifdef ORION_GRAPHICS_API_OPENGL
#include "glad/glad.h"
#else
#error "Unknown Graphics API!"
#endif // ORION_GRAPHICS_API_OPENGL

namespace orion
{
	enum texture_format_t : u8
	{
		// 3 channel
		texture_format_rgb8,
		texture_format_rgb16,
		texture_format_rgb32f,

		// 4 channel
		texture_format_rgba8,
		texture_format_rgba16,
		texture_format_rgba32f,
	};

	enum texture_flags_t
	{
		texture_flags_none             = 0 << 0,
		texture_flags_generate_mipmaps = 0 << 1,
	};

	/* Descriptor of a Texture. */
	struct texture_desc_t
	{
		texture_format_t format;
		texture_flags_t  flags;
		u16 width;
		u16 height;
	};

	/**
	 * @brief Represents given graphics API's underlying texture object.
	 */
	struct texture_t
	{
	#ifdef ORION_GRAPHICS_API_OPENGL
		using id_t          = GLuint;
		using type_t        = GLenum;
		using format_t      = GLenum;
		using base_format_t = GLenum;
	#else
		#error "Unknown Graphics API!"
	#endif // ORION_GRAPHICS_API_OPENGL

		id_t          id;
		type_t        type;
		format_t      format;
		base_format_t base_format;
		u16           width;
		u16           height;

		/**
		 * @brief Creates new 2D Texture from given descriptor and data on the GPU side.
		 * @param desc texture descriptor.
		 * @param data data that should be passed into GPU side.
		 * @param size size of data in bytes.
		 * @return newly created texurue.
		 */
		static texture_t create_texture_2d(const texture_desc_t& desc, const void* data, u32 size);

		/** Destroys given texture on the GPU side. */
		void destroy() const;

		/** Binds texture for use on the GPU side. */
		void bind() const;

		/** Unbinds texture from use on the GPU side. */
		void unbind() const;

		/**
		 * Transfers new data to the GPU.
		 *
		 * @important If the new data would exceed capacity,
		 * then new texture will be allocated with the previous data destroyed.
		 * Beware, this might be costly!
		 *
		 * @param data data to be transferred to the GPU.
		 * @param size size of the data in bytes.
		 */
		void set_data(const void* data, u32 size);

		/** Returns channel count for the current texutre. */
		u8 channel_count() const;
	};
}
