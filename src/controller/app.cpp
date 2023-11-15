#include "app.h"

void App::mouse_callback(GLFWwindow *window, double xOffset, double yOffset)
{
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
}

void App::scroll_callback(GLFWwindow *window, double xOffset, double yOffset)
{
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    float oldFOV = app->cameraSystem->fov;
    float newFOV = oldFOV - static_cast<float>(yOffset);

    if (newFOV < 1.0f)
        newFOV = 1.0f;
    else if (newFOV > 90.0f)
        newFOV = 90.0f;
    app->cameraSystem->fov = oldFOV - static_cast<float>(yOffset);
}

App *App::instance = nullptr;

void App::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (!instance)
        return;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        ImGuiIO &io = ImGui::GetIO();
        if (io.WantCaptureMouse)
        {
            std::cout << "imgui want mouse";
        }
        else
        {
            instance->cameraSystem->isFirstMouse = true;
            glfwSetCursorPos(window, 400, 300);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        instance->cameraSystem->isFirstMouse = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

App::App() : deltaTime(0.0f), lastFrameTime(0.0f), factory(new Factory(this->physicsComponents, this->renderComponents, this->transformComponents)),
             game(nullptr, 12345)
{
    instance = this;
    this->show_demo_window = true;

    unsigned int cameraEntityID = factory->make_camera({-5.0f, 5.0f, 4.0f}, {0.0f, 0.0f, 0.0f});

    CameraComponent *cameraComponent = new CameraComponent();
    cameraComponent->targetPos = glm::vec3(0.0f);
    cameraComponent->position = glm::vec3(0.0f);
    this->cameraComponent = cameraComponent;
    this->cameraID = cameraEntityID;

    this->game = Game(this->cameraComponent, 12345);

    init_glfw();
}

App::~App()
{
    for (Shader *shader : this->shaders)
    {
        shader->unbind();
    }

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

    window = glfwCreateWindow(800, 600, "Learning OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char *glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    this->clear_color = glm::vec4(0.9f, 1.0f, 0.7f, 1.00f);
}

void App::init_opengl()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // int w, h;
    // glfwGetFramebufferSize(window, &w, &h);
    // glViewport(0, 0, w, h);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    Shader *defaultShader = new Shader("../src/shaders/vertex.glsl", "../src/shaders/fragment.glsl");
    Shader *lightShader = new Shader("../src/shaders/light/vertex.glsl", "../src/shaders/light/fragment.glsl");
    shaders.push_back(defaultShader);
    shaders.push_back(lightShader);

    // must use program before making uniform calls
    // shaders[0]->bind();
    // shaders[0]->setInt("material", 0);
}

void App::init_systems()
{
    this->motionSystem = new MotionSystem();
    this->cameraSystem = new CameraSystem(window);
    this->renderSystem = new RenderSystem(shaders, window);

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            this->factory->make_chunk({i * 16, j * 16}, "../src/textures/big_chungus.jpg", 0);
        }
    }
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
        // renderSystem->update(transformComponents, renderComponents);

        cameraSystem->update(transformComponents, cameraID, *cameraComponent, deltaTime);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuiIO &io = ImGui::GetIO();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::Begin("Options");

            ImGui::SliderFloat("FOV", &cameraSystem->fov, 1.0f, 90.0f); // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float *)&clear_color);    // Edit 3 floats representing a color

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

            glm::vec3 cameraPos = cameraComponent->position;
            ImGui::Text("Camera Position  %.2f x  %.2f y %.2f z",
                        cameraPos.x, cameraPos.y, cameraPos.z);
            if (ImGui::Button("Camera Mode"))
            {
                if (cameraSystem->mode == FPS)
                {
                    cameraSystem->mode = ORBIT;
                }
                else
                {
                    cameraSystem->mode = FPS;
                }
            }
            ImGui::SameLine();
            ImGui::Text(cameraSystem->mode == FPS ? "FPS" : "ORBIT");
            ImGui::End();
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderSystem->update(transformComponents, renderComponents, *cameraComponent, cameraID);

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        // glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        handleInput();
        glfwPollEvents();
    }
}