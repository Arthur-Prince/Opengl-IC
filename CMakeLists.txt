cmake_minimum_required(VERSION 3.19)


set (CMAKE_CXX_STANDARD 17)

project(OpenGL)

add_executable(OpenGL stb_image/stb_image.cpp Vertex.h VertexArray.h Shader.h glad.c App.cpp)

target_link_libraries(OpenGL GL dl glfw)
