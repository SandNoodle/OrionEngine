cmake_minimum_required(VERSION 3.0)

project(stb)

add_library(${PROJECT_NAME} include/stb_image.h include/stb_image_write.h)
target_include_directories(${PROJECT_NAME} PUBLIC include/)
