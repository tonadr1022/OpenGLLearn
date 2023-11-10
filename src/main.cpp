#include "config.h"
#include "controller/app.h"
#include "factories/factory.h"

int main()
{
    App *app = new App();
    Factory *factory = new Factory(app->physicsComponents, app->renderComponents, app->transformComponents);
    factory->make_cube({0.0f, 0.0f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 10.0f});

    unsigned int cameraEntityID = factory->make_camera({0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f});

    CameraComponent *cameraComponent = new CameraComponent();
    app->cameraComponent = cameraComponent;
    app->cameraID = cameraEntityID;

    app->init_opengl();
    app->init_systems();
    app->run();

    delete app;
    delete factory;
    return 0;
}
