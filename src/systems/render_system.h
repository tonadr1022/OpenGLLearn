#pragma once
#include "../config.h"
#include "../view/shader.h"
#include "../components/transform_component.h"
#include "../components/render_component.h"

class RenderSystem
{
public:
    RenderSystem(Shader *shader, GLFWwindow *window);
    void update(
        std::map<unsigned int, TransformComponent> &transformComponents,
        std::map<unsigned int, RenderComponent> &renderComponents);

private:
    Shader *shader;
    GLFWwindow *window;
};