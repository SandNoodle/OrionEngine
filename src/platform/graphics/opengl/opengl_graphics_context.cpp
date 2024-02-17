#include "platform/graphics/graphics_context.h"

#ifdef ORION_GRAPHICS_API_OPENGL

#include "platform/graphics/opengl/opengl_native_types.h"

#include "core/assert.h"

namespace orion
{
	graphics_context_t graphics_context_t::create(void)
	{
		graphics_context_t c;
		glCreateVertexArrays(1, &c.vao);
		return c;
	}

	void graphics_context_t::destroy() const
	{
		glDeleteVertexArrays(1, &vao);
	}

	void graphics_context_t::draw_indexed(draw_attributes_t attributes) const
	{
		OE_ASSERT_TRUE(c, "Graphics Context cannot be NULL.");
		OE_ASSERT_TRUE(shader, "Shader cannot be NULL.");
		OE_ASSERT_TRUE(vertex_buffers, "Vertex Buffers cannot be NULL.");
		OE_ASSERT_TRUE(index_buffer, "Index Buffer cannot be NULL.");
		OE_ASSERT_TRUE(pipeline, "Pipeline cannot be NULL.");

		pipeline_execute(pipeline);

		glBindVertexArray(vao);

		shader->bind();
		framebuffer->bind();

		// Bind input vertex attributes
		for(size_t i = 0; i < shader->inputs_count; ++i)
		{
			const shader_element_t& element = shader->inputs[i];
			glEnableVertexArrayAttrib(vao, element.binding_index);
			glVertexArrayAttribBinding(vao, element.binding_index, 0);
			glVertexArrayAttribFormat(vao,
				element.binding_index,
				element.component_count,
				to_native_size(element.type),
				GL_FALSE,
				element.offset);
		}

		for(size_t i = 0; i < shader->samplers_count; ++i)
		{
			const texture_sampler_t& sampler = shader->samplers[i];
			glBindTextureUnit(sampler.binding_index, sampler.texture_id);
		}

		// Bind VBOs
		// @TODO Replace with glVertexArrayVertexBuffers.
		for(size_t i = 0; i < vertex_buffers_count; ++i)
		{
			glVertexArrayVertexBuffer(vao, i, vertex_buffers[i].id, 0, shader->_stride);
		}

		// Issue draw call
		// @TODO: Primitive topology (GL_TRAINGLES -> get_native_topology(pipeline.rasterizer.topology))
		glVertexArrayElementBuffer(vao, index_buffer->id);
		glDrawElements(GL_TRIANGLES, index_buffer->element_count, GL_UNSIGNED_INT, nullptr);

		// Cleanup
		framebuffer->unbind();
	}
}

#endif // ORION_GRAPHICS_API_OPENGL
