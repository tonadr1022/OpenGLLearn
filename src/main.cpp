#include "config.h"
#include "controller/app.h"
#include "factories/factory.h"
#include "factories/game/game.h"

int main()
{
    std::setprecision(10);
    App *app = new App();
    app->init_opengl();
    app->init_systems();
    app->run();

    delete app;
    return 0;
}
