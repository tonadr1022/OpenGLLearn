cmake_minimum_required(VERSION 3.5.0)
project(hello_window VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

include(CTest)
enable_testing()

find_package(OpenGL REQUIRED)

include_directories(src/vendor)


add_executable(hello_window 
    src/config.h 
    src/glad.c
    src/main.cpp 
    src/triangle_mesh.cpp
    src/config.cpp
    src/material.cpp
    src/systems/camera_system.cpp
    src/systems/motion_system.cpp
    src/systems/render_system.cpp
    src/view/shader.cpp
    src/controller/app.cpp
    src/factories/factory.cpp
    )

target_include_directories(hello_window PRIVATE dependencies)

# Specify the GLFW library path
find_library(GLFW_LIBRARY glfw)

target_link_libraries(hello_window ${GLFW_LIBRARY} ${OPENGL_gl_LIBRARY})

