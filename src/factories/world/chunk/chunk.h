#pragma once
#include "../block/block.h"
#include <array>
#include "chunklet.h"

class Chunk
{
public:
    Chunk();
    Chunk(glm::vec2 location);

    void setBlock(int x, int y, int z, Block block);
    Block getBlock(glm::ivec3 pos);
    void buildMesh();
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<Chunklet> chunklets;
    glm::ivec2 getLocation();

private:
    glm::ivec2 location;
    bool shouldAddFaceToMesh(const glm::ivec3 &adjBlockPos);
    void addFaceToMesh(const glm::ivec3 &blockPosition, const glm::ivec2 &chunkPosition, std::array<float, 32> blockFace);
    bool outOfBounds(const glm::ivec3 position);
};