cmake_minimum_required(VERSION 3.25)

project(OrionEngine_UT VERSION 0.0.1 LANGUAGES CXX)

# --- [ Globals ] ---
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

# --- [ Testing Setup ] ---
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
)
FetchContent_GetProperties(googletest)
if (NOT googletest_POPULATED)
    FetchContent_MakeAvailable(googletest)
endif ()
enable_testing()

# --- [ Sources ] ---
set(OrionEngine_UT_SOURCES
        Core/Standard/Algorithms/SortTest.cpp
        Core/Standard/Containers/ArrayTests.cpp
        Core/Standard/Containers/HashMapTests.cpp
        Core/Standard/Containers/SpanTests.cpp
        Core/Standard/Containers/StringTests.cpp
        Core/Standard/Containers/VectorTests.cpp
        Platform/FileSystem/Storage/LocalStorageTest.cpp
        TestCommon.h
)
add_executable(OrionEngine_UT ${OrionEngine_UT_SOURCES})
set_target_properties(OrionEngine_UT PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(OrionEngine_UT PRIVATE OrionEngine GTest::gtest_main)
target_include_directories(OrionEngine_UT PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# --- [ Test discovery ] ---
include(GoogleTest)
gtest_discover_tests(OrionEngine_UT)
