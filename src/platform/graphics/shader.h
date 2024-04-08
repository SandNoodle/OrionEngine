#pragma once

#include "platform/types.h"

#include "orion_config.h"

#include "platform/graphics/gpu_atomic_types.h"
#include "platform/graphics/buffer.h"

#ifdef ORION_GRAPHICS_API_OPENGL
#include "glad/glad.h"
#else
#error "Unknown Graphics API!"
#endif // ORION_GRAPHICS_API_OPENGL

#include <string_view>

namespace orion
{
	enum class shader_language : u8
	{
		glsl,
		spirv,
	};

	struct shader_element_t
	{
		u8 binding_index;
		u8 component_count;
		u16 offset;
		gpu_atomic_type type;
	};

	/**
	 * @brief TextureSampler is an object that represents a Texture as input to a Shader.
	 */
	struct texture_sampler_t
	{
		u32 binding_index;
		u32 texture_id;
	};

	/* Descriptor of a shader. */
	struct shader_desc_t
	{
		shader_language   language;
		texture_sampler_t samplers[ORION_GRAPHICS_MAX_TEXTURE_SAMPLERS];
		shader_element_t  inputs[ORION_GRAPHICS_MAX_SHADER_INPUTS];
		u8                samplers_count;
		u8                inputs_count;
	};

	/**
	 * @brief Represents given graphics API's underlying shader object.
	 */
	struct shader_t
	{
		#ifdef ORION_GRAPHICS_API_OPENGL
			using id_t    = GLuint;
		#else
			#error "Unknown Graphics API!"
		#endif // ORION_GRAPHICS_API_OPENGL

		id_t              id;
		texture_sampler_t samplers[ORION_GRAPHICS_MAX_TEXTURE_SAMPLERS];
		shader_element_t  inputs[ORION_GRAPHICS_MAX_SHADER_INPUTS];
		u8                samplers_count;
		u8                inputs_count;
		u16               _stride; // Stride between elements. Do NOT set directly.

		/**
		 * @brief Creates new shader from given descriptor and data on the GPU side.
		 * @param desc shader descriptor.
		 * @param vertex string containig vertex shader's data.
		 * @param fragment string containig fragmnet shader's data.
		 */
		static shader_t create_graphics_shader(const shader_desc_t& desc,
			                                   const std::string_view vertex,
			                                   const std::string_view fragment);

		/** Destroys given shader on the GPU side. */
		void destroy() const;

		/** Binds shader for use on the GPU side. */
		void bind() const;

		/** Unbinds shader from use on the GPU side. */
		void unbind() const;

		/**
		 * @brief Binds uniform buffer to a given shader.
		 * @param buffer uniform buffer to be bound to a shader.
		 * @param size index to bind the uniform buffer to.
		 */
		void set_uniform_buffer(const buffer_t& buffer, size_t index);
	};
}
