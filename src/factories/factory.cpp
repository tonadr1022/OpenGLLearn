#include "factory.h"
#include "../vendor/stb_image/stb_image.h"

Factory::Factory(
    std::map<unsigned int, PhysicsComponent> &physicsComponents,
    std::map<unsigned int, RenderComponent> &renderComponents,
    std::map<unsigned int, TransformComponent> &transformComponents) : physicsComponents(physicsComponents), renderComponents(renderComponents), transformComponents(transformComponents), entities_made(0) {}

Factory::~Factory()
{
    glDeleteBuffers(VBOs.size(), &VBOs[0]);
    glDeleteVertexArrays(VAOs.size(), &VAOs[0]);
    glDeleteTextures(textures.size(), &textures[0]);
}

unsigned int Factory::make_camera(glm::vec3 position, glm::vec3 eulers)
{

    TransformComponent transform;
    transform.position = position;
    transform.eulers = eulers;

    transformComponents[entities_made] = transform;

    return entities_made++;
}
struct Vertex
{
    float x, y, z;
    float nx, ny, nz; // normal vectors
    float s, t;       // texture coordinates
};

unsigned int Factory::make_sphere(glm::vec3 position, glm::vec3 eulers, glm::vec3 eulerVelocity)
{

    TransformComponent transform;
    transform.position = position;
    transform.eulers = eulers;
    transformComponents[entities_made] = transform;

    PhysicsComponent physics;
    physics.velocity = glm::vec3(0.0f);
    physics.eulerVelocity = eulerVelocity;
    physicsComponents[entities_made] = physics;

    RenderComponent render = make_sphere_mesh(1.0f);
    renderComponents[entities_made] = render;

    return entities_made++;
}

RenderComponent Factory::make_sphere_mesh(float radius)
{
    std::vector<Vertex> vertices;
    int stacks = 32;
    int slices = 32;
    for (int i = 0; i <= stacks; ++i)
    {
        float phi = static_cast<float>(i) / stacks * glm::pi<float>();
        float sinPhi = std::sin(phi);
        float cosPhi = std::cos(phi);

        for (int j = 0; j <= slices; ++j)
        {
            float theta = static_cast<float>(j) * 2.0f * glm::pi<float>() / slices;
            float sinTheta = std::sin(theta);
            float cosTheta = std::cos(theta);

            Vertex vertex;
            vertex.x = radius * cosTheta * sinPhi;
            vertex.y = radius * cosPhi;
            vertex.z = radius * sinTheta * sinPhi;

            vertex.nx = cosTheta * sinPhi;
            vertex.ny = cosPhi;
            vertex.nz = sinTheta * sinPhi;

            vertex.s = static_cast<float>(j) / slices;
            vertex.t = static_cast<float>(i) / stacks;

            vertices.push_back(vertex);
        }
    }
    // for (int i = 0; i < 20; i++)
    // {
    //     for (int j = 0; j < 40; j++)
    //     {
    //         float iRads = glm::radians(static_cast<float>(i));
    //         float jRads = glm::radians(static_cast<float>(j));

    //         float x = sin(iRads) * sin(jRads);
    //         float y = sin(iRads) * cos(jRads);
    //         float z = cos(jRads);

    //         vertices.push_back(glm::vec3(x, y, z) * radius);
    //     }
    // }
    // VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    VAOs.push_back(VAO);
    glBindVertexArray(VAO);

    // VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    VBOs.push_back(VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // set vertex attributes pointers
    // position vector
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);
    // texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)12);
    glEnableVertexAttribArray(1);
    // normal vector
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)20);
    glEnableVertexAttribArray(2);

    RenderComponent record;
    record.VAO = VAO;
    record.vertexCount = vertices.size();
    return record;
}

unsigned int Factory::make_cube(glm::vec3 position, glm::vec3 eulers, glm::vec3 eulerVelocity)
{
    TransformComponent transform;
    transform.position = position;
    transform.eulers = eulers;
    transformComponents[entities_made] = transform;

    PhysicsComponent physics;
    physics.velocity = glm::vec3(0.0f);
    physics.eulerVelocity = eulerVelocity;
    physicsComponents[entities_made] = physics;

    RenderComponent render = make_cube_mesh(glm::vec3(0.5f));
    render.shaderIndex = 1;
    render.material = make_texture("../src/textures/big_chungus.jpg");
    renderComponents[entities_made] = render;

    return entities_made++;
}

unsigned int Factory::make_texture(const char *filename)
{
    int width, height, channels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);

    unsigned int texture;

    // make the texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    textures.push_back(texture);

    // upload to GPU, describe layout on the GPU (mipmap, etc), then define data coming in (format, type, data)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // free data (it's already on the GPU now)
    stbi_image_free(data);

    // configure sampler
    // s = x-axis
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // t = y-axis
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // how to sample when image is shrunk down?
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // how to sample when image is blown up?
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}

RenderComponent Factory::make_cube_mesh(glm::vec3 size)
{
    float l = size.x;
    float w = size.y;
    float h = size.z;

    std::vector<float> vertices = {
        l, w, -h, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        l, -w, -h, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        -l, -w, -h, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        -l, -w, -h, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        -l, w, -h, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        l, w, -h, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,

        -l, -w, h, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        l, -w, h, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        l, w, h, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        l, w, h, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -l, w, h, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -l, -w, h, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        -l, w, h, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -l, w, -h, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -l, -w, -h, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -l, -w, -h, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -l, -w, h, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -l, w, h, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,

        l, -w, -h, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        l, w, -h, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        l, w, h, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        l, w, h, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        l, -w, h, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        l, -w, -h, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -l, -w, -h, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        l, -w, -h, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        l, -w, h, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        l, -w, h, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        -l, -w, h, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        -l, -w, -h, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,

        l, w, h, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        l, w, -h, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -l, w, -h, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -l, w, -h, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -l, w, h, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        l, w, h, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};

    // VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    VAOs.push_back(VAO);
    glBindVertexArray(VAO);

    // VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    VBOs.push_back(VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // set vertex attributes pointers
    // position vector
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, (void *)0);
    glEnableVertexAttribArray(0);
    // texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 32, (void *)12);
    glEnableVertexAttribArray(1);
    // normal vector
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 32, (void *)20);
    glEnableVertexAttribArray(2);

    RenderComponent record;
    record.VAO = VAO;
    record.vertexCount = 36;
    return record;
}