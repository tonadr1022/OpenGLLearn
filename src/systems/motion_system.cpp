#include "motion_system.h"

void MotionSystem::update(std::map<unsigned int, TransformComponent> &transformComponents,
                          std::map<unsigned int, PhysicsComponent> &physicsComponents, float deltaTime)
{
    for (auto &[entityID, physicsComponent] : physicsComponents)
    {
        // update position vec3 based on velocity and deltaTime
        transformComponents[entityID].position += physicsComponent.velocity * deltaTime;
        // update euler angles based on velocity and deltaTime
        transformComponents[entityID].eulers += physicsComponent.eulerVelocity * deltaTime;
        // start z rotation at 0 if surpasses 360
        if (transformComponents[entityID].eulers.z > 360)
        {
            transformComponents[entityID].eulers.z -= 360;
        }
    }
}
