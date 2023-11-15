#include "game.h"

int Game::getWorldSeed()
{
    return worldSeed;
}

Game::Game(CameraComponent *cameraComponent, int worldSeed) : world(cameraComponent, this), worldSeed(worldSeed) {}
