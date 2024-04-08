#include "platform/graphics/pipeline.h"

#ifdef ORION_GRAPHICS_API_OPENGL

#include "platform/graphics/opengl/opengl_native_types.h"

#include "core/assert.h"

namespace orion
{
	static GLenum get_depth_function(depth_function);
	static GLenum get_cull_mode(cull_mode);

	pipeline_t pipeline_t::create(const pipeline_desc_t& desc)
	{
		return (pipeline_t) {
			._desc = desc,
		};
	}

	void pipeline_t::execute() const
	{
		const depth_function depth_function = _desc.depth_stencil.depth_function;
		if (depth_function != depth_function::none)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(get_depth_function(depth_function));
			glDepthMask(_desc.depth_stencil.use_depth_mask ? GL_TRUE : GL_FALSE);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

		const cull_mode cull_mode = _desc.rasterizer.cull_mode;
		if(cull_mode != cull_mode::none)
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

	static GLenum get_depth_function(depth_function function)
	{
		switch(function)
		{
			case depth_function::never:         return GL_NEVER;
			case depth_function::always:        return GL_ALWAYS;
			case depth_function::less:          return GL_LESS;
			case depth_function::less_equal:    return GL_LEQUAL;
			case depth_function::equal:         return GL_EQUAL;
			case depth_function::not_equal:     return GL_NOTEQUAL;
			case depth_function::greater:       return GL_GREATER;
			case depth_function::greater_equal: return GL_GEQUAL;
			case depth_function::none:
				OE_UNREACHABLE("depth_function_none should be handled elsewhere.");
				return GL_NONE;
		}
		OE_UNREACHABLE("unhandled depth_function case");
		return GL_NONE;
	}

	static GLenum get_cull_mode(cull_mode mode)
	{
		switch(mode)
		{
			case cull_mode::back: [[likely]]; return GL_BACK;
			case cull_mode::front:            return GL_FRONT;
			case cull_mode::front_and_back:   return GL_FRONT_AND_BACK;
			case cull_mode::none:
				 OE_UNREACHABLE("cull_mode_none should be handled elsewhere.");
				 return GL_NONE;
		}
		OE_UNREACHABLE("unhandled null_mode case");
		return GL_NONE;
	}
}

#endif // ORION_GRAPHICS_API_OPENGL
