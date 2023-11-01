cmake_minimum_required(VERSION 3.0.0)
project(hello_window VERSION 1.0.0)

find_package(OpenGL REQUIRED)

add_executable(hello_window 
    src/config.h 
    src/main.cpp 
    src/glad.c)

target_include_directories(hello_window PRIVATE dependencies)

# Specify the GLFW library path
find_library(GLFW_LIBRARY glfw)

target_link_libraries(hello_window ${GLFW_LIBRARY} ${OPENGL_gl_LIBRARY})

