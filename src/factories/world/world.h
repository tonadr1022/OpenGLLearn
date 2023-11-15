#pragma once
// #include "../game/game.h"
#include "chunk/chunk_manager.h"
#include "../../components/camera_component.h"

class Game;

class World
{
public:
    World(CameraComponent *cameraComponent, Game *game);
    void RenderWorld();

private:
    ChunkManager chunkManager;
    void loadChunks();
    void updateChunks();

    CameraComponent *cameraComponent;
    Game *game;
};