#include "chunk/chunk_manager.h"
#include "../../components/camera_component.h"
class World
{
public:
    World(CameraComponent &cameraComponent);
    void RenderWorld();

private:
    ChunkManager chunkManager;
    void loadChunks();
    void updateChunks();

    CameraComponent &cameraComponent;
};