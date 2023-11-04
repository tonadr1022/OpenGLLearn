cmake_minimum_required(VERSION 3.5.0)
project(hello_window VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

find_package(OpenGL REQUIRED)

add_executable(hello_window 
    src/config.h 
    src/main.cpp 
    src/glad.c
    src/triangle_mesh.cpp
    src/triangle_mesh.h)

target_include_directories(hello_window PRIVATE dependencies)

# Specify the GLFW library path
find_library(GLFW_LIBRARY glfw)

target_link_libraries(hello_window ${GLFW_LIBRARY} ${OPENGL_gl_LIBRARY})

