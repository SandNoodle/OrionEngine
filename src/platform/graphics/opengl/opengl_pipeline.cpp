#include "platform/graphics/pipeline.h"

#ifdef ORION_GRAPHICS_API_OPENGL

#include "platform/compiler_macros.h"
#include "platform/memory.h"
#include "platform/graphics/opengl/opengl_native_types.h"

#include "core/assert.h"

namespace orion
{
	static GLenum get_depth_function(depth_function_t);
	static GLenum get_cull_mode(cull_mode_t);

	struct pipeline_t
	{
		pipeline_desc_t desc;
	};

	pipeline_t* pipeline_create(pipeline_desc_t desc)
	{
		pipeline_t* p = (pipeline_t*)platform_allocate(sizeof(pipeline_t));
		p->desc = desc;
		return p;
	}

	void pipeline_destroy(pipeline_t* p)
	{
		if(!p) return;
		platform_free(p);
	}

	void pipeline_execute(pipeline_t* p)
	{
		OE_ASSERT_TRUE(p, "Pipeline cannot be NULL.");

		depth_function_t depth_function = p->desc.depth_stencil.depth_function;
		if (depth_function != depth_function_none)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(get_depth_function(depth_function));
			glDepthMask(p->desc.depth_stencil.use_depth_mask ? GL_TRUE : GL_FALSE);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

		cull_mode_t cull_mode = p->desc.rasterizer.cull_mode;
		if(cull_mode != cull_mode_none)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(get_cull_mode(cull_mode));
			glFrontFace(p->desc.rasterizer.front_counter_clockwise ? GL_CCW : GL_CW);
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
