#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include "config.h"
#include "shader.h"

class CameraSystem
{
public:
    CameraSystem(Shader *shader, GLFWwindow *window);
    void mouse_callback(GLFWwindow *window, double xpos, double ypos);
    void processMouseMovement(float xOffset, float yOffset);
    void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);
    void update(float deltaTime);
    void setFOV(float fov);
    float getFOV();

private:
    Shader *shader;
    GLFWwindow *window;
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    float yaw;
    float pitch;
    float fov;
    float cameraSpeed;
};

#endif