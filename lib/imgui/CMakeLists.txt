cmake_minimum_required(VERSION 3.10)

project(imgui VERSION 1.84)

#Platforms
set(PLATFORM_GLFW platform/glfw/imgui_impl_glfw.cpp platform/glfw/imgui_impl_glfw.h)

#Renderers
set(RENDERER_OPENGL renderer/opengl/imgui_impl_opengl3_loader.h renderer/opengl/imgui_impl_opengl3.cpp renderer/opengl/imgui_impl_opengl3.h)

# ImGui files
set(IMGUI_HEADERS imconfig.h imgui_internal.h imgui.h imstb_rectpack.h imstb_textedit.h imstb_truetype.h)
set(IMGUI_SOURCES imgui_demo.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp imgui.cpp)

add_library(${PROJECT_NAME} ${PLATFORM_GLFW} ${RENDERER_OPENGL} ${IMGUI_HEADERS} ${IMGUI_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC /)

# GLFW + GLAD Dependencies
target_include_directories(${PROJECT_NAME} PUBLIC ${glfw_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${glad_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} GLAD)
