#include "terrain_generator.h"
#include "../world_constants.h"
#include "../../config.h"

TerrainGenerator::TerrainGenerator()
{
    // Create and configure FastNoise object
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
}
float TerrainGenerator::getPerlinNoise(int x, int z)
{
    float scale = 0.1f;       // Adjust this for terrain scale
    float persistence = 0.5f; // Adjust this for terrain variation
    float frequency = 0.1f;   // Adjust this for terrain frequency

    // Use glm::perlin function for 2D Perlin noise
    return glm::simplex(glm::vec2(x * scale, z * scale) * frequency) * persistence;
}

void TerrainGenerator::generateTerrainFor(Chunk &chunk)
{
    glm::ivec2 worldOffset = chunk.getLocation();
    // Gather noise data
    // std::vector<float> noiseData(CHUNK_SIZE * CHUNK_SIZE * 25);

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            // float noiseVal = noise.GetNoise((float)worldOffset.x + x, (float)worldOffset.y + y);
            int worldX = worldOffset.x + x;
            int worldY = worldOffset.y + y;
            float noiseVal = glm::simplex(glm::vec2(worldX, worldY) / 32.0f);
            noiseVal = (noiseVal + 1) / 2;

            int height = floor(noiseVal * 32);

            for (int z = 0; z < CHUNK_SIZE * chunk.chunklets.size(); z++)
            {
                if (z < height)
                {
                    chunk.chunklets[z / CHUNK_SIZE].setBlock(x, y, z % CHUNK_SIZE, BlockId::Dirt);
                }
                else
                {
                    chunk.chunklets[z / CHUNK_SIZE].setBlock(x, y, z % CHUNK_SIZE, BlockId::Air);
                }
            }
            // std::cout << height << '\n';
        }
    }
}