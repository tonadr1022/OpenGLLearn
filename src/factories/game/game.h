#include "../../config.h"
#include "../world/world.h"
#include "../../components/camera_component.h"

class Game
{
public:
    Game(CameraComponent &cameraComponent);
    glm::vec3 getPlayerPosition();
    void setPlayerPosition();
    void initWorld(int seed);

private:
    glm::vec3 playerPosition;
    World world;
};