#include "platform/graphics/pipeline.h"

#ifdef ORION_GRAPHICS_API_OPENGL

#include "platform/graphics/opengl/opengl_native_types.h"

#include "core/assert.h"

namespace orion
{
	static GLenum get_depth_function(depth_function_t);
	static GLenum get_cull_mode(cull_mode_t);

	pipeline_t pipeline_t::create(const pipeline_desc_t& desc)
	{
		return (pipeline_t) {
			._desc = desc,
		};
	}

	void pipeline_t::execute() const
	{
		depth_function_t depth_function = _desc.depth_stencil.depth_function;
		if (depth_function != depth_function_none)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(get_depth_function(depth_function));
			glDepthMask(_desc.depth_stencil.use_depth_mask ? GL_TRUE : GL_FALSE);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

		cull_mode_t cull_mode = _desc.rasterizer.cull_mode;
		if(cull_mode != cull_mode_none)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(get_cull_mode(cull_mode));
			glFrontFace(_desc.rasterizer.front_counter_clockwise ? GL_CCW : GL_CW);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
	}

	//
	//
	//

	static GLenum get_depth_function(depth_function_t function)
	{
		switch(function)
		{
			case depth_function_never:         return GL_NEVER;
			case depth_function_always:        return GL_ALWAYS;
			case depth_function_less:          return GL_LESS;
			case depth_function_less_equal:    return GL_LEQUAL;
			case depth_function_equal:         return GL_EQUAL;
			case depth_function_not_equal:     return GL_NOTEQUAL;
			case depth_function_greater:       return GL_GREATER;
			case depth_function_greater_equal: return GL_GEQUAL;
			case depth_function_none:
				OE_UNREACHABLE("depth_function_none should be handled elsewhere.");
		}
		OE_UNREACHABLE("unhandled depth_function case");
	}

	static GLenum get_cull_mode(cull_mode_t mode)
	{
		switch(mode)
		{
			case cull_mode_back: [[likely]]; return GL_BACK;
			case cull_mode_front:            return GL_FRONT;
			case cull_mode_front_and_back:   return GL_FRONT_AND_BACK;
			case cull_mode_none:
				 OE_UNREACHABLE("cull_mode_none should be handled elsewhere.");
		}
		OE_UNREACHABLE("unhandled null_mode case");
	}
}

#endif // ORION_GRAPHICS_API_OPENGL
