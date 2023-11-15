#include "render_system.h"
#include "../shaders/shader_manager.h"
RenderSystem::RenderSystem(std::vector<Shader *> shaders, GLFWwindow *window) : window(window), shaders(shaders) {}

void RenderSystem::update(
    std::map<unsigned int, TransformComponent> &transformComponents,
    std::map<unsigned int, RenderComponent> &renderComponents,
    CameraComponent &cameraComponent, unsigned int cameraID)
{
    for (auto &[entityID, renderComponent] : renderComponents)
    {
        glm::mat4 viewMatrix = ShaderManager::getViewMatrix();
        glm::mat4 projectionMatrix = ShaderManager::getProjectionMatrix();

        TransformComponent &transform = transformComponents[entityID];
        glm::mat4 modelMatrix = glm::mat4(1.0f);

        float time = static_cast<float>(glfwGetTime()) * 10.0f;

        // translate model matrix based on entity position for rendering
        modelMatrix = glm::translate(modelMatrix, transform.position);

        // std::cout << renderComponent.material << "index \n";
        Shader *shader = shaders[renderComponent.shaderIndex];
        // if (renderComponent.shaderIndex == 1)
        // {
        //     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // }

        shader->bind();
        shader->setInt("material", 0);
        shader->setVec3("viewPos", transformComponents[cameraID].position);
        shader->setMat4("view", viewMatrix);
        shader->setMat4("projection", projectionMatrix);
        shader->setMat4("model", modelMatrix);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderComponent.material);
        glBindVertexArray(renderComponent.VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderComponent.EBO);

        // glDrawArrays(GL_TRIANGLES, 0, renderComponent.vertexCount);
        glDrawElements(GL_TRIANGLES, renderComponent.vertexCount, GL_UNSIGNED_INT, 0);
        // if (renderComponent.shaderIndex == 1)
        // {
        //     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        // }
    }
}