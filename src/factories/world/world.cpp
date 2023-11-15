#include "world.h"
#include "../game/game.h"

World::World(CameraComponent *cameraComponent, Game *game) : cameraComponent(cameraComponent), game(game)
{
}

void World::RenderWorld()
{
    int worldSeed = game->getWorldSeed();
    glm::ivec3 initPos = cameraComponent->position;
}

void World::loadChunks()
{
    // need player position

    // get chunks within range

    // load each of them
}

void World::updateChunks()
{
}
