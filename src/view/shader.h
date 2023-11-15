#ifndef SHADER_H
#define SHADER_H

#include "../config.h"

class Shader
{
public:
    unsigned int ID;

    Shader(const char *vertexPath, const char *fragmentPath);

    void bind();
    void unbind();

    void setUniformBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 &matrix) const;
    void setVec3(const std::string &name, glm::vec3 &vector) const;

    unsigned int getUniformLocation(const std::string &name) const;

private:
    unsigned int makeModule(const std::string &filepath, unsigned int module_type);
    mutable std::unordered_map<std::string, unsigned int> UniformLocationCache;
};

#endif // SHADER_H