#pragma once
#include "../config.h"

class ShaderManager
{
public:
    static void setViewMatrix(glm::mat4 viewMatrix);
    static void setProjectionMatrix(glm::mat4 projectionMatrix);
    static glm::mat4 getViewMatrix();
    static glm::mat4 getProjectionMatrix();

private:
    static glm::mat4 viewMatrix;
    static glm::mat4 projectionMatrix;
};