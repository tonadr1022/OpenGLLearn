#include "../config.h"
#include "camera_system.h"

CameraSystem::CameraSystem(Shader *shader, GLFWwindow *window) : shader(shader), window(window), fov(45.0f), cameraSpeed(2.5f), mouseSensitivity(0.1f)
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
    glm::vec3 dEulers = glm::vec3(0.0f);
    double mouse_x, mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
    glfwSetCursorPos(window, 320.0, 240.0);

    dEulers.z = mouseSensitivity * static_cast<float>(mouse_x - 320.0);
    dEulers.y = mouseSensitivity * static_cast<float>(mouse_y - 240.0);

    if (dEulers.y + eulers.y > 89.0f)
        dEulers.y = 89.0f;
    else if (dEulers.y + eulers.y < -89.0f)
        dEulers.y = -89.0f;
    else
        dEulers.y = eulers.y + dEulers.y;

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

    /*
        using math convention where
        theta = angle from pos x axis toward y axis
        phi = angle from pos z axis toward y axis float
    */
    float theta = glm::radians(eulers.y);
    float phi = glm::radians(eulers.z);

    glm::vec3 &right = cameraComponent.right;
    glm::vec3 &up = cameraComponent.up;
    glm::vec3 &forwards = cameraComponent.fowards;

    float forwardsX = sin(phi) * cos(theta);
    float forwardsY = sin(phi) * sin(theta);
    float forwardsZ = cos(phi);
    forwards = {forwardsX, forwardsY, forwardsZ};

    glm::mat4 view = glm::lookAt(position, position + forwards, up);
    shader->setMat4("view", view);

    glm::mat4 projection = glm::perspective(glm::radians(fov), 640.0f / 480.0f, 0.1f, 100.0f);
    shader->setMat4("projection", projection);

    handleKeyInput(position, cameraComponent);
    // handleMouseMovement(eulers);
}