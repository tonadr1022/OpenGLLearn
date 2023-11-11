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