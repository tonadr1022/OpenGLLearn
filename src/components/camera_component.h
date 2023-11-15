#pragma once
#include "../config.h"

struct CameraComponent
{
    glm::vec3 targetPos;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 fowards;
    glm::vec3 position;
};