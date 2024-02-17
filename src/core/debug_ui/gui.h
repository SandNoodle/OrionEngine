#pragma once

#include "platform/types.h"

typedef struct GLFWwindow GLFWwindow;

namespace orion
{
	using window_handle = GLFWwindow*;

	/** */
	b8 debug_ui_create(window_handle* handle);

	/** */
	void debug_ui_destroy();

	/** */
	void debug_ui_prepare_frame();

	/** */
	void debug_ui_render_frame();
}
