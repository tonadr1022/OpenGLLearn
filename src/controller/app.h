#pragma once
#include "../config.h"

#include "../components/camera_component.h"
#include "../components/physics_component.h"
#include "../components/render_component.h"
#include "../components/transform_component.h"

#include "../systems/camera_system.h"
#include "../systems/motion_system.h"
#include "../systems/render_system.h"

#include "../view/shader.h"
class App
{
public:
    App();
    ~App();
    void run();
    void init_opengl();
    void init_systems();
    void handleInput();

    // Component Structures
    std::map<unsigned int, TransformComponent> transformComponents;
    std::map<unsigned int, PhysicsComponent> physicsComponents;
    std::map<unsigned int, RenderComponent> renderComponents;
    CameraComponent *cameraComponent;
    unsigned int cameraID;

private:
    static App *instance;
    GLFWwindow *window;
    void init_glfw();
    glm::vec4 clear_color;
    bool show_demo_window;
    void updateDeltaTime();

    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    static void mouse_callback(GLFWwindow *window, double xOffset, double yOffset);
    static void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);

    float deltaTime;
    float lastFrameTime;

    std::vector<Shader *> shaders;
    MotionSystem *motionSystem;
    CameraSystem *cameraSystem;
    RenderSystem *renderSystem;

    // void init_glfw();

    // float mixValue;
    // float deltaTime;
    // float lastFrameTime;
    // bool isFirstMouseRegister;
    // float lastMousePosX;
    // float lastMousePosY;
    // float mouseSensitivity;
    // GLFWwindow *window;
    // CameraSystem *cameraSystem;

    // void updateDeltaTime();

    // static void mouse_callback(GLFWwindow *window, double mousePosX, double mousePosY);
    // static void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);
};