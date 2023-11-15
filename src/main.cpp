#include "config.h"
#include "controller/app.h"
#include "factories/factory.h"
#include "factories/game/game.h"

int main()
{
    std::setprecision(10);
    App *app = new App();
    Factory *factory = new Factory(app->physicsComponents, app->renderComponents, app->transformComponents);

    unsigned int cameraEntityID = factory->make_camera({-5.0f, 5.0f, 4.0f}, {0.0f, 0.0f, 0.0f});

    CameraComponent *cameraComponent = new CameraComponent();
    cameraComponent->targetPos = glm::vec3(0.0f);
    app->cameraComponent = cameraComponent;
    app->cameraID = cameraEntityID;

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            factory->make_chunk({i * 16, j * 16}, "../src/textures/big_chungus.jpg", 0);
        }
    }
    // Game game{*cameraComponent};
    // game.initWorld(123456);

    app->init_opengl();
    app->init_systems();
    app->run();

    delete app;
    delete factory;
    return 0;
}
