cmake_minimum_required(VERSION 3.5.0)
project(hello_window VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

include(CTest)
enable_testing()

find_package(OpenGL REQUIRED)


include_directories(src/vendor)
include_directories(src/vendor/imgui)
include_directories(src/vendor/fast_noise_lite)


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
    src/shaders/shader_manager.cpp
    src/controller/app.cpp
    src/factories/factory.cpp
    src/factories/entity_manager.cpp
    src/factories/world/chunk/chunklet.cpp
    src/factories/world/chunk/chunk.cpp
    src/factories/world/block/block.cpp
    src/factories/game/game.cpp
    src/factories/world/world.cpp
    src/factories/world/generation/terrain_generator.cpp
    src/vendor/imgui/imgui.cpp
    src/vendor/imgui/imgui_demo.cpp
    src/vendor/imgui/imgui_draw.cpp
    src/vendor/imgui/imgui_widgets.cpp
    src/vendor/imgui/imgui_impl_glfw.cpp
    src/vendor/imgui/imgui_impl_opengl3.cpp
    src/vendor/imgui/imgui_tables.cpp
    src/vendor/imgui/imgui_widgets.cpp
    )

target_include_directories(hello_window PRIVATE dependencies)

# Specify the GLFW library path
find_library(GLFW_LIBRARY glfw)

target_link_libraries(hello_window ${GLFW_LIBRARY} ${OPENGL_gl_LIBRARY})

