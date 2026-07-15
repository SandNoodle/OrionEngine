cmake_minimum_required(VERSION 3.25)

project(OrionEditor VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

set(OrionEditor_SOURCES
        Sources/Main.cpp
)

add_executable(OrionEditor ${OrionEditor_SOURCES})
set_target_properties(OrionEditor PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(OrionEngine PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sources>
        $<INSTALL_INTERFACE:OrionEditor>
)

target_link_libraries(OrionEditor PUBLIC OrionEngine)

if (ORION_BUILD_TESTS)
    add_subdirectory(Tests)
endif ()
