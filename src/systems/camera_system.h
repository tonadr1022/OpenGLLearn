#pragma once
#include "../config.h"
#include "../view/shader.h"
#include "../components/transform_component.h"
#include "../components/camera_component.h"

class CameraSystem
{
public:
    CameraSystem(Shader *shader, GLFWwindow *window);

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
    bool isFirstMouse;
};