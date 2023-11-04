#include "config.h"
#include "triangle_mesh.h"

unsigned int make_shader(const std::string &vertex_filepath, const std::string &fragment_filepath);

unsigned int make_module(const std::string &filepath, unsigned int module_type);

int main()
{
    GLFWwindow *window;

    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    // set window as current openGL context
    glfwMakeContextCurrent(window);

    // tell glad to find openGL function definitions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // set color to clear the screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    TriangleMesh *mesh = new TriangleMesh();

    // create shader program
    unsigned int shader = make_shader("../src/shaders/vertex.glsl",
                                      "../src/shaders/fragment.glsl");

    while (!glfwWindowShouldClose(window))
    {
        // poll events
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        mesh->draw();

        glfwSwapBuffers(window);
    }

    // delete shader program
    glDeleteProgram(shader);

    glfwTerminate();

    return 0;
}

unsigned int make_shader(const std::string &vertex_filepath, const std::string &fragment_filepath)
{
    //  to store shader modules
    std::vector<unsigned int> shaderModules;

    // add vertex shader module
    shaderModules.push_back(make_module(vertex_filepath, GL_VERTEX_SHADER));

    // add fragment shader module
    shaderModules.push_back(make_module(fragment_filepath, GL_FRAGMENT_SHADER));

    // create shader program and attach modules to it
    unsigned int shaderProgram = glCreateProgram();
    for (unsigned int shaderModule : shaderModules)
    {
        glAttachShader(shaderProgram, shaderModule);
    }
    // link the program to create executables that will run on the GPU shaders
    glLinkProgram(shaderProgram);

    // check the linking worked
    int success;
    //  returns in params the value of a parameter for a specific program object.
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    // if linking failed, print error log
    if (!success)
    {
        char errorLog[1024];
        glGetProgramInfoLog(shaderProgram, 1024, NULL, errorLog);
        std::cout << "Shader Program linking error:\n"
                  << errorLog << std::endl;
    }

    // delete shader modules after linking them to the program
    for (unsigned int shaderModule : shaderModules)
    {
        glDeleteShader(shaderModule);
    }

    return shaderProgram;
}

unsigned int make_module(const std::string &filepath, unsigned int module_type)
{
    // open file and read contents into string stream (one big string)
    std::ifstream file;
    std::stringstream bufferedLines;
    std::string line;

    file.open(filepath);
    while (std::getline(file, line))
    {
        bufferedLines << line << '\n';
    }
    std::string shaderSource = bufferedLines.str();
    // to c style string
    const char *shaderSrc = shaderSource.c_str();
    bufferedLines.str(""); // erase string stream
    file.close();

    // compile shader modujle
    unsigned int shaderModule = glCreateShader(module_type);
    glShaderSource(shaderModule, 1, &shaderSrc, NULL);
    glCompileShader(shaderModule);

    // check success
    int success;
    glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char errorLog[1024];
        glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
        std::cout << "Shader Module compilation error:\n"
                  << errorLog << std::endl;
    }
    else
    {
        std::cout << "Shader Module compilation success: " << filepath << std::endl;
    }
    // std::cout << "Shader Module compilation success: " << filepath << std::endl;
    return shaderModule;
}