#pragma once

#include "orion_config.h"

#include "platform/types.h"
#include "platform/graphics/texture_format.h"

#ifdef ORION_GRAPHICS_API_OPENGL
#include "glad/glad.h"
#endif // ORION_GRAPHICS_API_OPENGL

namespace orion
{
	/** Descriptor of a Framebuffer. */
	struct framebuffer_desc_t
	{
		texture_format color_texture_formats[ORION_GRAPHICS_MAX_TEXTURE_SAMPLERS];
		texture_format depth_texture_format;
		usize color_texture_formats_count;
		u32 width;
		u32 height;
	};

	/**
	 * @brief Represents given graphics API's underlying framebuffer object.
	 */
	struct framebuffer_t
	{
	#ifdef ORION_GRAPHICS_API_OPENGL
		using id_t     = GLuint;
		using color_format_t = texture_format;
		using depth_format_t = texture_format;
	#else
		#error "Unknown Graphics API!"
	#endif // ORION_GRAPHICS_API_OPENGL

		id_t           id;
		id_t           depth_texture_id;
		id_t           color_textures_ids[ORION_GRAPHICS_MAX_TEXTURE_SAMPLERS];
		color_format_t color_textures_formats[ORION_GRAPHICS_MAX_TEXTURE_SAMPLERS];
		depth_format_t depth_texture_format;
		u8             color_textures_count;
		u16            width;
		u16            height;

		/** Creates new buffer from given descriptor on the GPU side. */
		static framebuffer_t create(const framebuffer_desc_t& desc);

		/** Destroys given framebuffer on the GPU side. */
		void destroy() const;

		/** Binds framebuffer for use on the GPU side. */
		void bind() const;

		/** Unbinds framebuffer from use on the GPU side. */
		void unbind() const;

		/**
		 * @brief Clears framebuffers's color texture.
		 * @param index index of a color texture
		 * @param color color to which texture will be cleared.
		 */
		void clear_color_texture(usize index, const float* color) const;

		/**
		 * @brief Clears all framebuffers's color textures.
		 * @param color color to which all textures will be cleared.
		 * @TODO Remove that pointer hack and replace it with a color struct.
		 */
		void clear_color_textures(const f32* color) const;

		/** Clears framebuffers's depth texture (if exists). */
		void clear_depth_texture() const;

		/** Resizes given framebuffer */
		void resize(u16 width, u16 height);

		/** Destroys and then recreates given framebuffer. */
		b8 recreate(u16 width, u16 height);
	};
}
