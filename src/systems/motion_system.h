#pragma once
#include "../config.h"
#include "../components/transform_component.h"
#include "../components/physics_component.h"

class MotionSystem
{
public:
    void update(std::map<unsigned int, TransformComponent> &transformComponents,
                std::map<unsigned int, PhysicsComponent> &physicsComponents, float deltaTime);
};