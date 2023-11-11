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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
    }
}

void App::init_opengl()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    this->shader = new Shader("../src/shaders/vertex.glsl", "../src/shaders/fragment.glsl");

    // must use program before making uniform calls
    shader->bind();
    shader->setInt("material", 0);
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
    }
}