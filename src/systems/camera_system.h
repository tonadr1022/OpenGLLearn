#pragma once
#include "../config.h"
#include "../view/shader.h"
#include "../components/transform_component.h"
#include "../components/camera_component.h"

enum CameraMode
{
    FPS,
    ORBIT,
};

class CameraSystem
{
public:
    CameraSystem(GLFWwindow *window);

    void update(std::map<unsigned int, TransformComponent> &transformComponents,
                unsigned int cameraEntityID, CameraComponent &cameraComponent, float deltaTime);
    void updateTarget(CameraComponent &cameraComponent, glm::vec3 targetPos);
    void handleKeyInput(glm::vec3 &position, CameraComponent &cameraComponent);
    void handleMouseMovement(glm::vec3 &eulers);
    bool isFirstMouse;
    float fov;
    CameraMode mode;

private:
    GLFWwindow *window;
    glm::vec3 global_up = {0.0f, 0.0f, 1.0f};

    float cameraSpeed;
    float mouseSensitivity;

    bool mouseEnabled;
};