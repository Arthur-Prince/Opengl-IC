cmake_minimum_required(VERSION 3.16)

set (CMAKE_CXX_STANDARD 17)

project(OpenGL)

add_executable(OpenGL stb_image/stb_image.cpp Shader.h glad.c Textura.cpp)

target_link_libraries(OpenGL GL dl glfw)
