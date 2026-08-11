cmake_minimum_required(VERSION 3.25)

project(OrionEngine VERSION 0.0.1 LANGUAGES CXX)

# --- [ Globals ] ---
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

# --- [ Options ] ---
set(ORION_GRAPHICS_API "Vulkan" CACHE STRING "Graphics API backed to be used; Possible values: Vulkan, None")

# --- [ Sources ] ---
set(OrionEngine_SOURCES
        Sources/Core/Console/ConsoleSystem.cpp
        Sources/Core/Console/ConsoleSystem.h
        Sources/Core/Memory/Allocators/Allocator.h
        Sources/Core/Memory/Allocators/ArenaAllocator.h
        Sources/Core/Memory/Allocators/DummyAllocator.h
        Sources/Core/Memory/Allocators/PlatformAllocator.h
        Sources/Core/Memory/MemorySystem.cpp
        Sources/Core/Memory/MemorySystem.h
        Sources/Core/Standard/Algorithms/Compare.h
        Sources/Core/Standard/Algorithms/Hash.h
        Sources/Core/Standard/Algorithms/Sort.h
        Sources/Core/Standard/Containers/Detail/StringFwd.h
        Sources/Core/Standard/Containers/Array.h
        Sources/Core/Standard/Containers/HashMap.h
        Sources/Core/Standard/Containers/Pair.h
        Sources/Core/Standard/Containers/Span.h
        Sources/Core/Standard/Containers/String.h
        Sources/Core/Standard/Containers/StringView.h
        Sources/Core/Standard/Containers/Vector.h
        Sources/Core/Standard/Memory/Lifetime.h
        Sources/Core/Standard/Utility/MathUtils.h
        Sources/Core/Standard/Utility/MoveAndForward.h
        Sources/Core/Standard/Utility/StringUtils.h
        Sources/Core/Standard/Utility/Swap.h
        Sources/Core/Standard/Concepts.h
        Sources/Core/Standard/EnumFlag.h
        Sources/Core/Standard/Limits.h
        Sources/Core/Standard/TypeTraits.h
        Sources/Core/Assert.h
        Sources/Core/Log/Logger.cpp
        Sources/Core/Log/Logger.h
        Sources/Platform/Graphics/Vulkan/RenderDeviceVk.cpp
        Sources/Platform/Graphics/Vulkan/RenderDeviceVk.h
        Sources/Platform/Graphics/RenderDevice.cpp
        Sources/Platform/Graphics/RenderDevice.h
        Sources/Platform/Platform.Android.cpp
        Sources/Platform/Platform.Linux.cpp
        Sources/Platform/Platform.Win32.cpp
        Sources/Platform/Platform.h
        Sources/OrionConfig.h
        Sources/OrionEngine.h
        Sources/EngineEntry.cpp
        Sources/EngineEntry.h
)

add_library(OrionEngine STATIC ${OrionEngine_SOURCES})
set_target_properties(OrionEngine PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(OrionEngine PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sources>
        $<INSTALL_INTERFACE:OrionEngine>
)
target_compile_options(OrionEngine PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wformat -Wconversion -Wimplicit-fallthrough -Werror=format-security -fno-exceptions>
)

# --- [ GIT ] ---
find_package(Git)
if (GIT_Found)
    execute_process(
            COMMAND git log -1 --format=%h
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
            OUTPUT_VARIABLE ORION_ENGINE_GIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
else ()
    set(ORION_ENGINE_GIT_HASH "<GIT REPOSITORY NOT FOUND>")
endif ()

# --- [ Defines ] ---
target_compile_definitions(OrionEngine PUBLIC
        ORION_ENGINE_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        ORION_ENGINE_VERSION_MINOR=${PROJECT_VERSION_MINOR}
        ORION_ENGINE_VERSION_PATCH=${PROJECT_VERSION_PATCH}
        ORION_ENGINE_GIT_HASH=${ORION_ENGINE_GIT_HASH}
        $<$<PLATFORM_ID:Windows>:ORION_PLATFORM_WINDOWS>
        $<$<PLATFORM_ID:Linux>:ORION_PLATFORM_LINUX>
        $<$<CONFIG:Debug>:ORION_BUILD_DEBUG>
        $<$<CONFIG:Release>:ORION_BUILD_RELEASE>
)

# --- [ Libraries ] ---
message(STATUS "--- [ Library: GLFW ] ---")
add_subdirectory(ThirdParty/GLFW)

target_link_libraries(OrionEngine
        PUBLIC
        glfw
)

# --- [ Graphics API ] ---
set_property(CACHE ORION_GRAPHICS_API PROPERTY STRINGS "Vulkan" "None")
if (${ORION_GRAPHICS_API} STREQUAL "Vulkan")
    message(STATUS "--- [ Graphics API: Vulkan ] ---")
    target_compile_definitions(OrionEngine PUBLIC ORION_GRAPHICS_VULKAN)
    add_subdirectory(ThirdParty/VMA)
    add_subdirectory(ThirdParty/Volk)
    find_package(Vulkan REQUIRED)
    target_link_libraries(OrionEngine
            PUBLIC
            GPUOpen::VulkanMemoryAllocator
            Vulkan::Vulkan
            volk::volk
    )
elseif (${ORION_GRAPHICS_API} STREQUAL "None")
    target_compile_definitions(OrionEngine PUBLIC ORION_GRAPHICS_NONE)
endif ()

# --- [ Testing ] ---
if (ORION_BUILD_TESTS)
    add_subdirectory(Tests)
endif ()

# --- [ Build Info ] ---
string(TOUPPER ${CMAKE_BUILD_TYPE} ORION_ENGINE_BUILD_TYPE)
string(TIMESTAMP ORION_ENGINE_BUILD_TIME "%Y-%m-%d %H:%M:%S" UTC)

message(STATUS "")
message(STATUS "-------------------------------- [ BUILD INFO ] --------------------------------")
message(STATUS " -> Engine Name           : ${PROJECT_NAME}")
message(STATUS " -> Engine Version        : ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
message(STATUS " -> Engine Graphics API   : ${ORION_GRAPHICS_API}")
message(STATUS " -> Engine Build Date     : ${ORION_ENGINE_BUILD_TIME}")
message(STATUS " -> Engine Git Hash       : ${ORION_ENGINE_GIT_HASH}")
message(STATUS "--------------------------------------------------------------------------------")
message(STATUS " -> Build Type            : ${CMAKE_BUILD_TYPE}")
message(STATUS " -> Build Host Platform   : ${CMAKE_HOST_SYSTEM}")
message(STATUS " -> Build Target Platform : ${CMAKE_SYSTEM}")
message(STATUS " -> Build Tests           : ${ORION_BUILD_TESTS}")
message(STATUS "--------------------------------------------------------------------------------")
message(STATUS " -> C Compiler            : ${CMAKE_C_COMPILER} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS " -> CXX Compiler          : ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS " -> Linker                : ${CMAKE_LINKER}")
message(STATUS "--------------------------------------------------------------------------------")
message(STATUS " -> C Flags               : ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_${ORION_ENGINE_BUILD_TYPE}}")
message(STATUS " -> CXX Flags             : ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${ORION_ENGINE_BUILD_TYPE}}")
message(STATUS " -> Linker Flags          : ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_${ORION_ENGINE_BUILD_TYPE}}")
message(STATUS "--------------------------------------------------------------------------------")
message(STATUS "")
