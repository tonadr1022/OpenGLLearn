#pragma once
#include "../config.h"
#include "../components/camera_component.h"
#include "../components/physics_component.h"
#include "../components/render_component.h"
#include "../components/transform_component.h"

class Factory
{
public:
    Factory(
        std::map<unsigned int, PhysicsComponent> &physicsComponents,
        std::map<unsigned int, RenderComponent> &renderComponents,
        std::map<unsigned int, TransformComponent> &transformComponents);
    ~Factory();

    unsigned int make_camera(glm::vec3 position, glm::vec3 eulers);
    unsigned int make_cube(glm::vec3 position, glm::vec3 eulers, glm::vec3 eulerVelocity);
    unsigned int make_sphere(glm::vec3 position, glm::vec3 eulers, glm::vec3 eulerVelocity);

private:
    unsigned int entities_made;

    std::map<unsigned int, PhysicsComponent> &physicsComponents;
    std::map<unsigned int, RenderComponent> &renderComponents;
    std::map<unsigned int, TransformComponent> &transformComponents;

    std::vector<unsigned int> VAOs;
    std::vector<unsigned int> VBOs;
    std::vector<unsigned int> EBOs;
    std::vector<unsigned int> textures;

    RenderComponent make_sphere_mesh(float radius);
    RenderComponent make_cube_mesh(glm::vec3 size);
    unsigned int make_texture(const char *filename);
};