#pragma once
#include "../chunk/chunk.h"

class TerrainGenerator
{
public:
    TerrainGenerator();
    void generateTerrainFor(Chunk &chunk);
    FastNoiseLite noise;
    float getPerlinNoise(int x, int y);
};