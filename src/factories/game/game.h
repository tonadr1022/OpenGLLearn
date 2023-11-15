#pragma once
#include "../../config.h"
#include "../../components/camera_component.h"
#include "../world/world.h"

class Game
{
public:
    Game(CameraComponent *cameraComponent, int worldSeed);
    int getWorldSeed();

private:
    glm::vec3 playerPosition;
    World world;
    int worldSeed;
};
