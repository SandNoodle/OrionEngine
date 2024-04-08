#pragma once

#include "platform/types.h"

#ifdef ORION_GRAPHICS_API_OPENGL
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#else
#error "Unknown Graphics API!"
#endif // ORION_GRAPHICS_API_OPENGL

namespace orion
{
	/** Descriptor of a Window. */
	struct window_desc_t
	{
		const char* title;
		u16         width;
		u16         height;
		b8          is_visible;
		b8          is_vsync;
		b8          is_fullscreen;
		b8          is_resizable;
	};

	/**
	 * @brief Represents window we can display to.
	 */
	struct window_t
	{
	#ifdef ORION_GRAPHICS_API_OPENGL
		using handle_t = GLFWwindow*;
	#else
		#error "Unknown Graphics API!"
	#endif // ORION_GRAPHICS_API_OPENGL

		window_desc_t desc;
		handle_t      handle;

		/**
		 * @brief Creates new window from given descriptor.
		 * @param desc window descriptor.
		 * @return True if window was created, false otherwise.
		 */
		static window_t create(const window_desc_t& desc);

		/** Destroys given window. */
		void destroy() const;

		/** Polls pending events for this window. */
		void poll_events() const;

		/** Swaps front buffer and back buffer for this window. */
		void swap_buffers() const;

		void resize(u16 width, u16 height);

		void set_title(const char* title);
		void set_visible(b8 visible);
		void set_resizable(b8 resizable);
		void set_fullscreen(b8 fullscreen);
		void set_vsync(b8 vsync);

		b8 get_should_close() const;
	};
}
