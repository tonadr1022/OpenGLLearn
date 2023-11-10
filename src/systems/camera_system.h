#pragma once
#include "../config.h"
#include "../view/shader.h"
#include "../components/transform_component.h"
#include "../components/camera_component.h"

class CameraSystem
{
public:
    CameraSystem(Shader *shader, GLFWwindow *window);
    // void mouse_callback(GLFWwindow *window, double xpos, double ypos);
    // void processMouseMovement(float xOffset, float yOffset);
    // void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);
    // void update(float deltaTime);
    // void setFOV(float fov);
    // float getFOV();
    void update(std::map<unsigned int, TransformComponent> &transformComponents,
                unsigned int cameraEntityID, CameraComponent &cameraComponent, float deltaTime);
    void handleKeyInput(glm::vec3 &position, CameraComponent &cameraComponent);
    void handleMouseMovement(glm::vec3 &eulers);

private:
    Shader *shader;
    GLFWwindow *window;
    glm::vec3 global_up = {0.0f, 0.0f, 1.0f};
    float fov;
    float cameraSpeed;
    float mouseSensitivity;
    // glm::vec3 cameraPos;
    // glm::vec3 cameraFront;
    // glm::vec3 cameraUp;
    // float yaw;
    // float pitch;
};
// class CameraSystem
// {
// public:
//     CameraSystem(Shader *shader, GLFWwindow *window);
//     void mouse_callback(GLFWwindow *window, double xpos, double ypos);
//     void processMouseMovement(float xOffset, float yOffset);
//     void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);
//     void update(float deltaTime);
//     void setFOV(float fov);
//     float getFOV();

// private:
//     Shader *shader;
//     GLFWwindow *window;
//     glm::vec3 cameraPos;
//     glm::vec3 cameraFront;
//     glm::vec3 cameraUp;

//     float yaw;
//     float pitch;
//     float fov;
//     float cameraSpeed;
// };