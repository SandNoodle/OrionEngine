#include "platform/graphics/window.h"

#ifdef ORION_GRAPHICS_API_OPENGL

#include "core/log.h"

namespace orion
{
#if BUILD_DEBUG
	static void register_debug_callbacks();
#endif // BUILD_DEBUG

	window_t window_t::create(const window_desc_t& desc)
	{
		b8 glfw_success = glfwInit();
		if(!glfw_success)
		{
			OE_LOG_FATAL("Initialization of GLFW failed!");
			return {};
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		#if BUILD_DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		#endif

		glfwWindowHint(GLFW_RESIZABLE, desc.is_resizable ? GLFW_TRUE : GLFW_FALSE);

		window_t w;
		w.title         = desc.title;
		w.width         = desc.width;
		w.height        = desc.height;
		w.is_visible    = desc.is_visible;
		w.is_vsync      = desc.is_vsync;
		w.is_fullscreen = desc.is_fullscreen;
		w.is_resizable  = desc.is_resizable;
		w.handle = glfwCreateWindow(desc.width, desc.height, desc.title, nullptr, nullptr);

		if(!w.handle)
		{
			OE_LOG_FATAL("Window creation failed, because cannot acquire a handle!");
			return {};
		}

		glfwMakeContextCurrent(w.handle);

		b8 glad_success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if(!glad_success)
		{
			OE_LOG_FATAL("Initialization of GLAD failed!");
			return {};
		}
		glViewport(0, 0, desc.width, desc.height);

		w.set_visible(desc.is_visible);
		w.set_vsync(desc.is_vsync);

		// Center the resulting window
		const GLFWvidmode* primary_monitor = glfwGetVideoMode(glfwGetPrimaryMonitor());
		if(primary_monitor)
		{
			glfwSetWindowPos(w.handle,
				(primary_monitor->width - desc.width) / 2,
				(primary_monitor->height - desc.height) / 2);
		}

		#if BUILD_DEBUG
		register_debug_callbacks();
		#endif // BUILD_DEBUG

		return w;
	}

	void window_t::destroy() const
	{
		glfwDestroyWindow(handle);
		glfwTerminate();
	}

	void window_t::poll_events() const
	{
		glfwPollEvents();
	}

	void window_t::swap_buffers() const
	{
		glfwSwapBuffers(handle);
	}

	void window_t::resize(u16 width, u16 height)
	{
		glfwSetWindowSize(handle, width, height);
		glViewport(0, 0, width, height);
		this->width = width;
		this->height = height;
	}

	void window_t::set_title(const char* title)
	{
		glfwSetWindowTitle(handle, title);
		this->title = title;
	}

	void window_t::set_visible(b8 visible)
	{
		is_visible = visible;
		visible ? glfwShowWindow(handle) : glfwHideWindow(handle);
	}

	void window_t::set_resizable(b8 resizable)
	{
		is_resizable = resizable;
		glfwSetWindowAttrib(handle, GLFW_RESIZABLE, resizable ? GL_TRUE : GL_FALSE);
	}

	void window_t::set_fullscreen(b8 fullscreen)
	{
		is_fullscreen = fullscreen;
		// @TODO Fullscreen
	}

	void window_t::set_vsync(b8 vsync)
	{
		is_vsync = vsync;
		glfwSwapInterval(vsync);
	}

	b8 window_t::get_should_close() const
	{
		return glfwWindowShouldClose(handle);
	}

	//
	//
	//

	const char* error_type_string(GLenum type)
	{
		switch(type)
		{
			case GL_DEBUG_TYPE_ERROR:                return "Error";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:  return "Deprecated Behaviour";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:   return "Undefined Behaviour";
			case GL_DEBUG_TYPE_PORTABILITY:          return "Portability";
			case GL_DEBUG_TYPE_PERFORMANCE:          return "Performance";
			case GL_DEBUG_TYPE_MARKER:               return "Marker";
			case GL_DEBUG_TYPE_PUSH_GROUP:           return "Push Group";
			case GL_DEBUG_TYPE_POP_GROUP:            return "Pop Group";
			case GL_DEBUG_TYPE_OTHER:                return "Other";
			default:                                 return "Unknown";
		}
	}

	const char* error_source_string(GLenum source)
	{
		switch(source)
		{
			case GL_DEBUG_SOURCE_API:             return "API";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "Window System";
			case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
			case GL_DEBUG_SOURCE_THIRD_PARTY:     return "Third Party";
			case GL_DEBUG_SOURCE_APPLICATION:     return "Application";
			case GL_DEBUG_SOURCE_OTHER:           return "Other";
			default:                              return "Unknown";
		}
	}

	void APIENTRY opengl_debug_callback(
		GLenum source,
		GLenum type,
		unsigned int id,
		GLenum severity,
		GLsizei,
		const char* message,
		const void*)
	{
		const char* type_string = error_type_string(type);
		const char* source_string = error_source_string(source);
		switch(severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:
				OE_LOG_FATAL("OpenGL |  High  | Type: {} | ID: {} | Source: {} | Message: {}",
					type_string, id, source_string, message);
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				OE_LOG_ERROR("OpenGL | Medium | Type: {} | ID: {} | Source: {} | Message: {}",
					type_string, id, source_string, message);
				break;
			case GL_DEBUG_SEVERITY_LOW:
				OE_LOG_WARN("OpenGL |  Low   | Type: {} | ID: {} | Source: {} | Message: {}",
					type_string, id, source_string, message);
				break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				OE_LOG_WARN("OpenGL | Notify | Type: {} | ID: {} | Source: {} | Message: {}",
					type_string, id, source_string, message);
				break;
		}
	}

	static void register_debug_callbacks()
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(opengl_debug_callback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, false);
	}

}

#endif // ORION_GRAPHICS_API_OPENGL
