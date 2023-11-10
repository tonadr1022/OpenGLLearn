#include "config.h"

class Cube
{
public:
    Cube(glm::vec3 size);
    void draw();
    ~Cube();

private:
    unsigned int EBO, VAO, vertex_count;
    std::vector<unsigned int> VBOs;
};