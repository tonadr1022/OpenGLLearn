#include "../config.h"
#include "shader.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    unsigned int vertexShaderModule = makeModule(vertexPath, GL_VERTEX_SHADER);
    unsigned int fragmentShaderModule = makeModule(fragmentPath, GL_FRAGMENT_SHADER);

    // create shader program and attach modules to it

    //  store shader modules to iterate
    std::vector<unsigned int> shaderModules;
    shaderModules.push_back(vertexShaderModule);
    shaderModules.push_back(fragmentShaderModule);

    // create shader program and attach modules to it
    ID = glCreateProgram();
    for (unsigned int shaderModule : shaderModules)
    {
        glAttachShader(ID, shaderModule);
    }
    // link the program to create executables that will run on the GPU shaders
    glLinkProgram(ID);

    // check the linking worked
    int success;
    //  returns in params the value of a parameter for a specific program object.
    glGetProgramiv(ID, GL_LINK_STATUS, &success);

    // if linking failed, print error log
    if (!success)
    {
        char errorLog[1024];
        glGetProgramInfoLog(ID, 1024, NULL, errorLog);
        std::cout << "Shader Program linking error:\n"
                  << errorLog << std::endl;
    }

    // delete shader modules after linking them to the program
    for (unsigned int shaderModule : shaderModules)
    {
        glDeleteShader(shaderModule);
    }
}

unsigned int Shader::makeModule(const std::string &filepath, unsigned int module_type)
{
    // open file and read contents into string stream (one big string)
    std::ifstream file;
    std::stringstream bufferedLines;
    std::string line;

    /*
    std::ifstream::failbit: This bit is set when a logical error occurs during the file input operation, such as trying to read past the end of the file or attempting to read data of the wrong type.

std::ifstream::badbit: This bit is set when a low-level I/O error occurs, such as failure to open the file or a hardware-level error.
    */

    // ensure the file can throw exceptions
    // file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
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

    // compile shader module
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
        std::cout << "Shader Module compilation error:" << filepath << "\n"
                  << errorLog << std::endl;
    }
    else
    {
        std::cout << "Shader Module compilation success: " << filepath << std::endl;
    }
    return shaderModule;
}

void Shader::bind()
{
    glUseProgram(ID);
}

void Shader::unbind()
{
    glDeleteProgram(ID);
}

unsigned int Shader::getUniformLocation(const std::string &name) const
{
    if (UniformLocationCache.find(name) != UniformLocationCache.end())
        return UniformLocationCache[name];

    unsigned int location = glGetUniformLocation(ID, name.c_str());
    UniformLocationCache[name] = location;
    return location;
}
void Shader::setUniformBool(const std::string &name, bool value) const
{
    unsigned int location = getUniformLocation(name);
    glUniform1i(location, (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
    unsigned int location = getUniformLocation(name);
    glUniform1i(location, value);
}
void Shader::setFloat(const std::string &name, float value) const
{
    unsigned int location = getUniformLocation(name);
    glUniform1f(location, value);
}
void Shader::setMat4(const std::string &name, glm::mat4 &matrix) const
{
    unsigned int location = getUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVec3(const std::string &name, glm::vec3 &vector) const
{
    unsigned int location = getUniformLocation(name);
    glUniform3f(location, vector[0], vector[1], vector[2]);
}
