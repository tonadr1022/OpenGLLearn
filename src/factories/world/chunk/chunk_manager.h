#include "chunk.h"
#include "../../../config.h"

class ChunkManager
{
public:
    void loadChunk(glm::ivec2 position);
    void unloadChunk(glm::ivec2 position);

private:
    std::unordered_map<glm::ivec2, Chunk> chunkMap;
};