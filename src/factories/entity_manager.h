#pragma once
#include "../config.h"
#include "../components/camera_component.h"
#include "../components/physics_component.h"
#include "../components/render_component.h"
#include "../components/transform_component.h"

class EntityManager
{
public:
    EntityManager();
    unsigned int createEntity();
    void destroyEntity(unsigned int entityId);

private:
    std::vector<int> freeList;
    std::vector<int> entities;
    std::unordered_map<unsigned int, unsigned int> sparseArray;

    unsigned int nextEntityId;

    std::map<unsigned int, PhysicsComponent> &physicsComponents;
    std::map<unsigned int, RenderComponent> &renderComponents;
    std::map<unsigned int, TransformComponent> &transformComponents;
};