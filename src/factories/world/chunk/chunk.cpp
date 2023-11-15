#include "chunk.h"
#include "../block/block.h"
#include "../world_constants.h"
#include <array>
#include "chunklet.h"

namespace
{
    const std::array<GLfloat, 32> frontFace{
        // front face
        1, 0, 0, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1, 1, 0, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1, 0, 1, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1, 1, 1, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        //
    };

    const std::array<GLfloat, 32> backFace{
        // back face
        0, 0, 0, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0, 1, 0, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0, 0, 1, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0, 1, 1, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        //
    };

    const std::array<GLfloat, 32> leftFace{
        // left face
        0, 0, 0, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1, 0, 0, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        0, 0, 1, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        1, 0, 1, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        //
    };

    const std::array<GLfloat, 32> rightFace{
        // rig1t face
        0, 1, 0, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1, 1, 0, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0, 1, 1, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1, 1, 1, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f
        //
    };

    const std::array<GLfloat, 32> topFace{
        // top face
        0, 0, 1, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        1, 0, 1, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0, 1, 1, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1, 1, 1, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        //
    };

    const std::array<GLfloat, 32> bottomFace{
        // bottom face
        0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1, 0, 0, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0, 1, 0, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        1, 1, 0, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        //
    };
} // namespace

struct AdjacentBlockPositions
{
    void update(int x, int y, int z)
    {
        up = {x, y, z + 1};
        down = {x, y, z - 1};
        back = {x - 1, y, z};
        front = {x + 1, y, z};
        left = {x, y - 1, z};
        right = {x, y + 1, z};
    }
    glm::ivec3 up;
    glm::ivec3 down;
    glm::ivec3 left;
    glm::ivec3 right;
    glm::ivec3 front;
    glm::ivec3 back;
};

Chunk::Chunk() : location(glm::vec3(0.0f))
{
}

Chunk::Chunk(glm::vec2 location) : location(location)
{

    for (int i = 0; i < 8; i++)
    {
        Chunklet c{glm::ivec3(location.x, location.y, 8 * i), this};
        chunklets.push_back(c);
    }
}

void Chunk::setBlock(int x, int y, int z, Block block)
{

    int remainderZ = z % CHUNK_SIZE;
    chunklets[z / CHUNK_SIZE].setBlock(x, y, remainderZ, block);
}

Block Chunk::getBlock(glm::ivec3 position)
{
    int chunkletIndex = position.z / CHUNK_SIZE;
    int chunkletZPos = position.z % CHUNK_SIZE;

    return chunklets[chunkletIndex].getBlock({position.x, position.y, chunkletZPos});
}

const glm::vec3 frontFaceNormal(0.0f, 0.0f, 1.0f);
const glm::vec3 backFaceNormal(0.0f, 0.0f, -1.0f);
const glm::vec3 leftFaceNormal(-1.0f, 0.0f, 0.0f);
const glm::vec3 rightFaceNormal(1.0f, 0.0f, 0.0f);
const glm::vec3 topFaceNormal(0.0f, 1.0f, 0.0f);
const glm::vec3 bottomFaceNormal(0.0f, -1.0f, 0.0f);

void Chunk::buildMesh()
{
    AdjacentBlockPositions adjBlockPositions;
    int chunkletIndex = 0;
    for (Chunklet &chunklet : chunklets)
    {
        // std::cout << chunklet.location.x << " " << chunklet.location.y << " " << chunklet.location.z << "chunklet loc\n";
        for (int i = 0; i < CHUNK_VOLUME; i++)
        {
            // coords in this chunklet, not world or chunk adjusted
            int z = i % CHUNK_SIZE;
            int y = (i / CHUNK_SIZE) % CHUNK_SIZE;
            int x = i / (CHUNK_SIZE * CHUNK_SIZE);

            if (chunklet.getBlock({x, y, z}).id == BlockId::Air)
            {
                continue;
            }
            int chunkZ = z + (chunkletIndex * CHUNK_SIZE);
            adjBlockPositions.update(x, y, chunkZ);

            if (shouldAddFaceToMesh(adjBlockPositions.up))
            {
                addFaceToMesh({x, y, chunkZ}, location, topFace);
            }
            if (shouldAddFaceToMesh(adjBlockPositions.down))
            {
                addFaceToMesh({x, y, chunkZ}, location, bottomFace);
            }
            if (shouldAddFaceToMesh(adjBlockPositions.right))
            {
                addFaceToMesh({x, y, chunkZ}, location, rightFace);
            }

            if (shouldAddFaceToMesh(adjBlockPositions.left))
            {
                addFaceToMesh({x, y, chunkZ}, location, leftFace);
            }
            if (shouldAddFaceToMesh(adjBlockPositions.front))
            {
                addFaceToMesh({x, y, chunkZ}, location, frontFace);
            }
            if (shouldAddFaceToMesh(adjBlockPositions.back))
            {
                addFaceToMesh({x, y, chunkZ}, location, backFace);
            }
        }
        chunkletIndex++;
    }
}

glm::ivec2 Chunk::getLocation()
{
    return location;
}
bool Chunk::outOfBounds(const glm::ivec3 position)
{
    if (position.x < 0 || position.x >= CHUNK_SIZE || position.y < 0 || position.y >= CHUNK_SIZE || position.z < 0 || position.z >= CHUNK_SIZE * chunklets.size())
        return true;
    return false;
}

bool Chunk::shouldAddFaceToMesh(const glm::ivec3 &adjBlockPos)
{
    if (outOfBounds(adjBlockPos))
        return true;
    if (getBlock(adjBlockPos).id == BlockId::Air)
        return true;
    return false;
}

void Chunk::addFaceToMesh(const glm::ivec3 &blockPosition, const glm::ivec2 &chunkPosition, std::array<float, 32> blockFace)
{
    glm::ivec3 globalBlockPosition = glm::ivec3(chunkPosition.x, chunkPosition.y, 0) + blockPosition;

    for (int i = 0; i < 32; i += 8)
    {
        blockFace[i] += static_cast<float>(globalBlockPosition.x);
        blockFace[i + 1] += static_cast<float>(globalBlockPosition.y);
        blockFace[i + 2] += static_cast<float>(globalBlockPosition.z);
    }

    vertices.insert(vertices.end(), blockFace.begin(), blockFace.end());

    // Update indices based on the current size of the vertices array
    unsigned int startIndex = static_cast<unsigned int>(vertices.size() / 8) - 4;
    indices.push_back(startIndex);
    indices.push_back(startIndex + 1);
    indices.push_back(startIndex + 3);
    indices.push_back(startIndex);
    indices.push_back(startIndex + 2);
    indices.push_back(startIndex + 3);
}