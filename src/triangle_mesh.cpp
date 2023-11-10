#include "triangle_mesh.h"
#include "config.h"

/*
    VAO - vertex array object: stores vertex attribute calls.
        can be bound just like a vertex buffer object and any subsequent vertex attribute calls from that point on will be stored inside the VAO. This has the advantage that when configuring vertex attribute pointers you only have to make those calls once and whenever we want to draw the object, we can just bind the corresponding VAO. This makes switching between different vertex data and attribute configurations as easy as binding a different VAO. All the state we just set is stored inside the VAO.

VBO - vertex buffer object: stores vertices in GPU memory
            advantage: send large dage to GPU once, draw many times

*/
Cube::Cube(glm::vec3 size)
{
    float l = size.x;
    float w = size.y;
    float h = size.z;

    std::vector<float> vertices = {
        l, w, -h, 1.0f, 1.0f,
        l, -w, -h, 1.0f, 0.0f,
        -l, -w, -h, 0.0f, 0.0f,
        -l, -w, -h, 0.0f, 0.0f,
        -l, w, -h, 0.0f, 1.0f,
        l, w, -h, 1.0f, 1.0f,

        -l, -w, h, 0.0f, 0.0f,
        l, -w, h, 1.0f, 0.0f,
        l, w, h, 1.0f, 1.0f,
        l, w, h, 1.0f, 1.0f,
        -l, w, h, 0.0f, 1.0f,
        -l, -w, h, 0.0f, 0.0f,

        -l, w, h, 1.0f, 1.0f,
        -l, w, -h, 1.0f, 0.0f,
        -l, -w, -h, 0.0f, 0.0f,
        -l, -w, -h, 0.0f, 0.0f,
        -l, -w, h, 0.0f, 1.0f,
        -l, w, h, 1.0f, 1.0f,

        l, -w, -h, 0.0f, 0.0f,
        l, w, -h, 1.0f, 0.0f,
        l, w, h, 1.0f, 1.0f,
        l, w, h, 1.0f, 1.0f,
        l, -w, h, 0.0f, 1.0f,
        l, -w, -h, 0.0f, 0.0f,

        -l, -w, -h, 0.0f, 0.0f,
        l, -w, -h, 1.0f, 0.0f,
        l, -w, h, 1.0f, 1.0f,
        l, -w, h, 1.0f, 1.0f,
        -l, -w, h, 0.0f, 1.0f,
        -l, -w, -h, 0.0f, 0.0f,

        l, w, h, 1.0f, 1.0f,
        l, w, -h, 1.0f, 0.0f,
        -l, w, -h, 0.0f, 0.0f,
        -l, w, -h, 0.0f, 0.0f,
        -l, w, h, 0.0f, 1.0f,
        l, w, h, 1.0f, 1.0f
        // first three are xyz positions, last two are uv tex coords

    };

    // std::vector<int> elements;
    // std::vector<int> baseElements = {0, 1, 2, 2, 1, 3};
    // for (int i = 0; i < 6; i++)
    // {
    //     for (int j = 0; j < 6; j++)
    //     {
    //         elements.push_back(baseElements[j] + (4 * i));
    //     }
    // }
    std::vector<int> colorIndices = {0, 1, 2, 3};

    vertex_count = 36;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    VBOs.resize(2);

    /*
    glGenBuffers returns n buffer object names in buffers. There is no guarantee that the names form a contiguous set of integers; however, it is guaranteed that none of the returned names was in use immediately before the call to glGenBuffers.
    */
    // glGenBuffers(2, VBOs.data());

    glGenBuffers(1, &VBOs[0]);
    /*
    Specifies the target to which the buffer object is bound, which must be one of the buffer binding targets
    */
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

    /*
    create new data store. Specify target in the table, size of data in bytes, pointer to data, and usage pattern.
    Static: the data store contents will be modified once and used many times.
    Draw: the data store contents are modified by the application, and used as the source for GL drawing and image specification commands.
    */
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    /* when we send data to GPU it doesn't know how to interpret it
     we need to tell it how to interpret it.
     we do this by setting vertex attribute pointers.
     we have 2 attributes: position and color.
     we need to tell openGL how to read each attribute.

     format: location (attribute number),
     size (num components per attribute),
     data type, whether to normalize,
     stride (space between consecutive vertex attributes),
     offset (where the attribute begins in the buffer) position
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // texture indices
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // // color indices
    // glGenBuffers(1, &VBOs[1]);
    // glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    // glBufferData(GL_ARRAY_BUFFER, colorIndices.size() * sizeof(int), colorIndices.data(), GL_STATIC_DRAW);
    // glVertexAttribIPointer(2, 1, GL_INT, 1 * sizeof(int), (void *)0);
    // glEnableVertexAttribArray(2);

    // // element buffer
    // glGenBuffers(1, &EBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(int), elements.data(), GL_STATIC_DRAW);
}

void Cube::draw()
{
    glBindVertexArray(VAO);
    // draw mode, first vertex, count
    glDrawArrays(GL_TRIANGLES, 0, vertex_count);

    // glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, 0);
}

Cube::~Cube()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, VBOs.data());

    // delete element buffer object
    glDeleteBuffers(1, &EBO);
}