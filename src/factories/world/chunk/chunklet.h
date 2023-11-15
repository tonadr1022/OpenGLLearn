#pragma once
#include "../../config.h"
#include "../world_constants.h"

class Chunk;

class Chunklet
{
public:
    Chunklet(glm::vec3 location, Chunk *pChunk);
    void setBlock(int x, int y, int z, Block block);
    Block getBlock(glm::ivec3 position);
    glm::vec3 location;

private:
        static int getIndex(int x, int y, int z);
    Chunk *pChunk;

    std::array<Block, CHUNK_VOLUME> blocks;
};