#include "app.h"

App::App() : deltaTime(0.0f), lastFrameTime(0.0f)
{
    init_glfw();
}

App::~App()
{
    shader->unbind();

    delete motionSystem;
    delete cameraSystem;
    delete renderSystem;
}

void App::init_glfw()
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Hello Window", NULL, NULL);

    glfwMakeContextCurrent(window);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return;
    }
}

void App::init_opengl()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    this->shader = new Shader("../src/shaders/vertex.glsl", "../src/shaders/fragment.glsl");
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);

    // must use program before making uniform calls
    shader->bind();
    shader->setInt("material", 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}

void App::init_systems()
{
    this->motionSystem = new MotionSystem();
    this->cameraSystem = new CameraSystem(shader, window);
    this->renderSystem = new RenderSystem(shader, window);
}

void App::updateDeltaTime()
{
    float currentFrameTime = glfwGetTime();
    this->deltaTime = currentFrameTime - lastFrameTime;
    this->lastFrameTime = currentFrameTime;
}

void App::handleInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void App::run()
{
    while (!glfwWindowShouldClose(window))
    {
        updateDeltaTime();
        renderSystem->update(transformComponents, renderComponents);
        cameraSystem->update(transformComponents, cameraID, *cameraComponent, deltaTime);
        renderSystem->update(transformComponents, renderComponents);
        handleInput();
        glfwPollEvents();

        // shader->setFloat("mixValue", mixValue);
    }
}

// App::App() : mixValue(0.7f),
//              deltaTime(0.0f),
//              lastFrameTime(0.0f),
//              isFirstMouseRegister(true),
//              mouseSensitivity(0.1f),
//              window(nullptr),
//              cameraSystem(nullptr){};

// void App::run()
// {

//     glfwSetCursorPosCallback(window, mouse_callback);
//     glfwSetScrollCallback(window, scroll_callback);

//     glfwMakeContextCurrent(window);
//     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//     {
//         std::cout << "Failed to initialize GLAD" << std::endl;
//         glfwTerminate();
//         return;
//     }

//     glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

//     Cube *mesh = new Cube(glm::vec3(0.5));
//     Material *material = new Material("../src/textures/big_chungus.jpg");
//     // Material *mask = new Material("../src/textures/mask.jpg");

//     // create shader program
//     Shader *shader = new Shader("../src/shaders/vertex.glsl", "../src/shaders/fragment.glsl");

//     // must use program before making uniform calls
//     shader->bind();
//     this->cameraSystem = new CameraSystem(shader, window);

//     shader->setInt("material", 0);

//     glm::vec3 cubePositions[] = {
//         glm::vec3(0.0f, 0.0f, 0.0f),
//         glm::vec3(2.0f, 5.0f, -15.0f),
//         glm::vec3(-1.5f, -2.2f, -2.5f),
//         glm::vec3(-3.8f, -2.0f, -12.3f),
//         glm::vec3(2.4f, -0.4f, -3.5f),
//         glm::vec3(-1.7f, 3.0f, -7.5f),
//         glm::vec3(1.3f, -2.0f, -2.5f),
//         glm::vec3(1.5f, 2.0f, -2.5f),
//         glm::vec3(1.5f, 0.2f, -1.5f),
//         glm::vec3(-1.3f, 1.0f, -1.5f)};

//     while (!glfwWindowShouldClose(window))
//     {
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//         glfwPollEvents();

//         updateDeltaTime();
//         cameraSystem->update(deltaTime);

//         // shader->setFloat("mixValue", mixValue);

//         shader->bind();
//         material->use(0);
//         for (unsigned int i = 0; i < 10; i++)
//         {
//             glm::mat4 model = glm::mat4(1.0f);
//             model = glm::translate(model, cubePositions[i]);
//             float angle = 20.0f * i + 30.0f;
//             model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
//             shader->setMat4("model", model);
//             mesh->draw();
//         }

//         glfwSwapBuffers(window);
//     }
//     shader->unbind();
//     delete mesh;
//     delete material;
// }

// void App::scroll_callback(GLFWwindow *window, double xOffset, double yOffset)
// {
//     App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
//     float oldFOV = app->cameraSystem->getFOV();
//     float newFOV = oldFOV - static_cast<float>(yOffset);

//     if (newFOV < 1.0f)
//         newFOV = 1.0f;
//     else if (newFOV > 90.0f)
//         newFOV = 90.0f;
//     std::cout << newFOV << '\n';
//     app->cameraSystem->setFOV(oldFOV - static_cast<float>(yOffset));
// }

// void App::mouse_callback(GLFWwindow *window, double mousePosX, double mousePosY)
// {
//     App *app = static_cast<App *>(glfwGetWindowUserPointer(window));

//     if (app->isFirstMouseRegister) // initially set to true
//     {
//         app->lastMousePosX = mousePosX;
//         app->lastMousePosY = mousePosY;
//         app->isFirstMouseRegister = false;
//     }
//     float mousePosXOffset = mousePosX - app->lastMousePosX;
//     float mousePosYOffset = app->lastMousePosY - mousePosY;
//     app->lastMousePosX = mousePosX;
//     app->lastMousePosY = mousePosY;

//     float xOffset = mousePosXOffset * app->mouseSensitivity;
//     float yOffset = mousePosYOffset * app->mouseSensitivity;
//     app->cameraSystem->processMouseMovement(xOffset, yOffset);
// }
