#include "../block/block.h"
#include "chunklet.h"
#include "../world_constants.h"
#include "chunk.h"

Chunklet::Chunklet(glm::vec3 location, Chunk *pChunk) : location(location), pChunk(pChunk) {}

void Chunklet::setBlock(int x, int y, int z, Block block)
{
    blocks[getIndex(x, y, z)] = block;
}

Block Chunklet::getBlock(glm::ivec3 position)
{
    return blocks[getIndex(position.x, position.y, position.z)];
}

int Chunklet::getIndex(int x, int y, int z)
{
    return y * CHUNK_AREA + z * CHUNK_SIZE + x;
}