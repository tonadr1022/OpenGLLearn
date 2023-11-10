#include "config.h"
#include "CameraSystem.h"

class App
{
public:
    App();
    ~App();

    void run();

private:
    float mixValue;
    float deltaTime;
    float lastFrameTime;
    bool isFirstMouseRegister;
    float lastMousePosX;
    float lastMousePosY;
    float mouseSensitivity;
    GLFWwindow *window;
    CameraSystem *cameraSystem;

    void updateDeltaTime();

    static void mouse_callback(GLFWwindow *window, double mousePosX, double mousePosY);
    static void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);
};