cmake_minimum_required(VERSION 3.25)

project(Orion)

option(ORION_BUILD_TESTS "Build tests" ON)

add_subdirectory(Engine)
add_subdirectory(Tools)
