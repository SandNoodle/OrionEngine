#include "platform/graphics/shader.h"

#ifdef ORION_GRAPHICS_API_OPENGL

#include "platform/graphics/opengl/opengl_native_types.h"

#include "core/assert.h"

namespace orion
{
	static void calculate_offsets_and_stride(shader_t& s);

	shader_t shader_t::create_graphics_shader(const shader_desc_t& desc,
		                                      const std::string_view vertex,
		                                      const std::string_view fragment)
	{
		shader_t s = {0};
		calculate_offsets_and_stride(s);

		GLuint vertex_id   = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
		switch(desc.language)
		{
			case shader_language::glsl:
				{
					glShaderSource(vertex_id, 1, (const char* const*)vertex.data(), nullptr);
					glShaderSource(fragment_id, 1, (const char* const*)fragment.data(), nullptr);
					glCompileShader(vertex_id);
					glCompileShader(fragment_id);
				}
				break;
			case shader_language::spirv:
				{
					glShaderBinary(
						1, &vertex_id,
						GL_SHADER_BINARY_FORMAT_SPIR_V,
						vertex.data(),
						vertex.length());
					glShaderBinary(
						1, &fragment_id,
						GL_SHADER_BINARY_FORMAT_SPIR_V,
						fragment.data(),
						fragment.length());
					glSpecializeShader(vertex_id, (const GLchar *) "main", 0, nullptr, nullptr);
					glSpecializeShader(fragment_id, (const GLchar *) "main", 0, nullptr, nullptr);
				}
				break;
			default:
				OE_UNREACHABLE();
		}

		GLint vertex_compiled = false;
		GLint fragment_compiled = false;
		glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &vertex_compiled);
		glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &fragment_compiled);

		if(!vertex_compiled) OE_LOG_ERROR("Failed to compile vertex shader.");
		if(!fragment_compiled) OE_LOG_ERROR("Failed to compile fragment shader.");

		s.id = glCreateProgram();
		glAttachShader(s.id, vertex_id);
		glAttachShader(s.id, fragment_id);
		glLinkProgram(s.id);

		GLint shader_linked = false;
		glGetShaderiv(s.id, GL_LINK_STATUS, &shader_linked);
		if(!shader_linked) OE_LOG_ERROR("Failed to link the shader.");

		glDetachShader(s.id, vertex_id);
		glDetachShader(s.id, fragment_id);
		glDeleteShader(vertex_id);
		glDeleteShader(fragment_id);

		const b8 compilation_succeed = vertex_compiled && fragment_compiled && shader_linked;
		if(!compilation_succeed)
		{
			OE_LOG_ERROR("Failed to compile standard shader.");
		}
		return s;
	}

	void shader_t::destroy() const
	{
		glUseProgram(0);
		glDeleteProgram(id);
	}

	void shader_t::bind() const
	{
		glUseProgram(id);
	}

	void shader_t::unbind() const
	{
		glUseProgram(0);
	}

	void shader_t::set_uniform_buffer(const buffer_t& b, usize index)
	{
		OE_ASSERT_TRUE(b.type == GL_UNIFORM_BUFFER, "Buffer is not of an UBO type.");
		glUniformBlockBinding(id, index, b.id);
		glBindBufferBase(GL_UNIFORM_BUFFER, index, b.id);
	}

	//
	//
	//

	static void calculate_offsets_and_stride(shader_t& s)
	{
		usize offset = 0;
		for(usize i = 0; i < s.inputs_count; ++i)
		{
			usize element_size =  to_native_size(s.inputs[i].type);
			usize final_size = s.inputs_count * element_size;
			s.inputs[i].offset = offset;
			s._stride += final_size;
			offset += final_size;
		}

	}
}

#endif // ORION_GRAPHICS_API_OPENGL
