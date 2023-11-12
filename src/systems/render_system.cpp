#include "render_system.h"

RenderSystem::RenderSystem(std::vector<Shader *> shaders, GLFWwindow *window) : window(window), shaders(shaders) {}

void RenderSystem::update(
    std::map<unsigned int, TransformComponent> &transformComponents,
    std::map<unsigned int, RenderComponent> &renderComponents)
{
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto &[entityID, renderComponent] : renderComponents)
    {
        TransformComponent &transform = transformComponents[entityID];
        glm::mat4 model = glm::mat4(1.0f);
        // translate model matrix based on entity position for rendering
        model = glm::translate(model, transform.position);

        // arbitrary rotation for now
        float time = static_cast<float>(glfwGetTime()) * 10.0f;
        model = glm::rotate(model, glm::radians(time), {0.0f, 0.0f, 1.0f});
        std::cout << renderComponent.shaderIndex << '\n';
        // shaders[renderComponent.shaderIndex]->bind();
        shaders[0]->setMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderComponent.material);
        glBindVertexArray(renderComponent.VAO);
        glDrawArrays(GL_TRIANGLES, 0, renderComponent.vertexCount);
    }
}