#include "core/debug_ui/gui.h"

#if 0

#ifdef ORION_GRAPHICS_API_OPENGL

#include "renderer/opengl/imgui_impl_opengl3.h"
#include "platform/glfw/imgui_impl_glfw.h"
#include "imgui.h"

namespace orion
{
	b8 debug_ui_create(window_handle* handle)
	{
		if(!handle) return false;

		// Setup ImGUI
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports;
		io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;

		io.ConfigWindowsMoveFromTitleBarOnly = true;
		io.ConfigDragClickToInputText = true;
		io.ConfigDockingTransparentPayload = true;

		// Theme and style
		ImGui::StyleColorsDark();
		// @TODO

		// Backends
		ImGui_ImplGlfw_InitForOpenGL(handle, true);
		ImGui_ImplOpenGL3_Init("#version 460");

		return true;
	}


	void debug_ui_destroy()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void debug_ui_prepare_frame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void debug_ui_render_frame()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}

#endif // ORION_GRAPHICS_API_OPENGL

#endif
