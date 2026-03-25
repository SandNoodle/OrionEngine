cmake_minimum_required(VERSION 3.25)

project(OrionEngine_UT VERSION 0.0.1 LANGUAGES CXX)

# --- [ Globals ] ---
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

# --- [ Doctest ] ---
find_package(doctest REQUIRED)
enable_testing()

# --- [ Sources ] ---
set(OrionEngine_UT_SOURCES
        Core/Standard/Containers/ArrayTests.cpp
        Core/Standard/Containers/SpanTests.cpp
        Core/Standard/Containers/VectorTests.cpp
)
add_executable(OrionEngine_UT ${OrionEngine_UT_SOURCES})
target_link_libraries(OrionEngine_UT PRIVATE OrionEngine)