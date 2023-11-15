#include "shader_manager.h"

glm::mat4 ShaderManager::viewMatrix = glm::mat4(1.0f);
glm::mat4 ShaderManager::projectionMatrix = glm::mat4(1.0f);

void ShaderManager::setViewMatrix(glm::mat4 viewMatrix)
{
    ShaderManager::viewMatrix = viewMatrix;
}

void ShaderManager::setProjectionMatrix(glm::mat4 projectionMatrix)
{
    ShaderManager::projectionMatrix = projectionMatrix;
}

glm::mat4 ShaderManager::getViewMatrix()
{
    return ShaderManager::viewMatrix;
}

glm::mat4 ShaderManager::getProjectionMatrix()
{
    return ShaderManager::projectionMatrix;
}
