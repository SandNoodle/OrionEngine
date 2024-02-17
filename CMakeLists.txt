cmake_minimum_required(VERSION 3.10)

# ----- [ Project ] -----
project(OrionEngine VERSION 0.1 LANGUAGES CXX)

# ----- [ Globals ] -----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ----- [ Options ] ------
# TODO: Select rendering API

# ----- [ Build Types ] ------
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo")
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}"
	CACHE STRING "List of supported configurations.")

if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type provided, defaulting to RELEASE.")
	set(CMAKE_BUILD_TYPE "Release")
endif()

if(NOT CMAKE_BUILD_TYPE IN_LIST CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Available buiild types: ${CMAKE_CONFIGURATION_TYPES}")
	message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")
endif()

# ----- [ Files ] ------
file (GLOB_RECURSE ${PROJECT_NAME}_SOURCES LIST_DIRECTORIES true CONFIGURE_DEPENDS "src/*.cpp")
file (GLOB_RECURSE ${PROJECT_NAME}_HEADERS LIST_DIRECTORIES true CONFIGURE_DEPENDS "src/*.h")

set (INCLUDE_DIRS "")
foreach (_headerFile ${${PROJECT_NAME}_HEADERS})
    get_filename_component(_dir ${_headerFile} DIRECTORY)
	list (APPEND INCLUDE_DIRS ${_dir})
endforeach()
list (REMOVE_DUPLICATES INCLUDE_DIRS)

# ----- [ Create Library ] ------
add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

# @TODO Temp force OpenGL as rendering api.
target_compile_definitions(${PROJECT_NAME} PUBLIC ORION_GRAPHICS_API_OPENGL)

# @TOOD Add Distribution configuration.
if(CMAKE_BUILD_TYPE STREQUAL Debug)
	target_compile_definitions(${PROJECT_NAME} PUBLIC ORION_BUILD_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL Release)
	target_compile_definitions(${PROJECT_NAME} PUBLIC ORION_BUILD_RELEASE)
endif()

# @TODO Display compile options

# ----- [ Link libraries ] -----
add_subdirectory(lib/glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

add_subdirectory(lib/glad)
target_link_libraries(${PROJECT_NAME} PUBLIC GLAD)

add_subdirectory(lib/ImGui)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui)

add_subdirectory(lib/stb)
set_target_properties(stb PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${PROJECT_NAME} PUBLIC stb)
