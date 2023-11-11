#include "../config.h"
#include "camera_system.h"

CameraSystem::CameraSystem(Shader *shader, GLFWwindow *window) : shader(shader), window(window), fov(45.0f), cameraSpeed(0.1f), mouseSensitivity(0.1f), isFirstMouse(true)
{
    shader->bind();
}

void CameraSystem::handleKeyInput(glm::vec3 &position, CameraComponent &cameraComponent)
{
    glm::vec3 forwards = cameraComponent.fowards;
    glm::vec3 up = cameraComponent.up;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += forwards * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= forwards * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= glm::normalize(glm::cross(forwards, up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += glm::normalize(glm::cross(forwards, up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        position += up * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        position -= up * cameraSpeed;
    }
}

void CameraSystem::handleMouseMovement(glm::vec3 &eulers)
{
    if (isFirstMouse)
    {
        glfwSetCursorPos(window, 320, 240);
        isFirstMouse = false;
        return;
    }
    glm::vec3 dEulers = glm::vec3(0.0f);
    double mouse_x, mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
    glfwSetCursorPos(window, 320.0, 240.0);

    dEulers.z = -mouseSensitivity * static_cast<float>(mouse_x - 320.0);
    dEulers.y = -mouseSensitivity * static_cast<float>(mouse_y - 240.0);

    if (dEulers.y + eulers.y > 89.0f)
        dEulers.y = 89.0f;
    else if (dEulers.y + eulers.y < -89.0f)
        dEulers.y = -89.0f;
    else
        eulers.y += dEulers.y;

    eulers.z += dEulers.z;

    if (eulers.z > 360)
    {
        eulers.z -= 360;
    }
    else if (eulers.z < 0)
    {
        eulers.z += 360;
    }
}

void CameraSystem::update(std::map<unsigned int, TransformComponent> &transformComponents, unsigned int cameraEntityID, CameraComponent &cameraComponent, float deltaTime)
{
    glm::vec3 &position = transformComponents[cameraEntityID].position;
    glm::vec3 &eulers = transformComponents[cameraEntityID].eulers;

    float theta = glm::radians(eulers.z);
    float phi = glm::radians(eulers.y);

    glm::vec3 &right = cameraComponent.right;
    glm::vec3 &up = cameraComponent.up;
    glm::vec3 &forwards = cameraComponent.fowards;

    forwards = {
        glm::cos(theta) * glm::cos(phi),
        glm::sin(theta) * glm::cos(phi),
        glm::sin(phi)};

    right = glm::normalize(glm::cross(forwards, global_up));
    up = glm::normalize(glm::cross(right, forwards));

    glm::mat4 view = glm::lookAt(position, position + forwards, up);
    shader->setMat4("view", view);

    glm::mat4 projection = glm::perspective(glm::radians(fov), 640.0f / 480.0f, 0.1f, 100.0f);
    shader->setMat4("projection", projection);

    handleKeyInput(position, cameraComponent);
    handleMouseMovement(eulers);
}