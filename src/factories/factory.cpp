#include "factory.h"
#include "../vendor/stb_image/stb_image.h"
#include "world/chunk/chunk.h"
#include "world/generation/terrain_generator.h"

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
    float s, t;       // texture coordinates
    float nx, ny, nz; // normal vectors
};

unsigned int Factory::make_sphere(glm::vec3 position, glm::vec3 eulers, glm::vec3 eulerVelocity, unsigned int shaderIndex)
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
    render.shaderIndex = shaderIndex;
    renderComponents[entities_made] = render;

    return entities_made++;
}

float distance(glm::vec3 a, glm::vec3 b)
{
    return sqrtf(dot(a, b));
}

glm::vec3 normalize(glm::vec3 a, glm::vec3 b, float length)
{
    // get distance between a and b along axes
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float dz = b.z = a.z;

    float currLength = distance(a, b);

    dx = dx * (length / currLength);
    dy = dy * (length / currLength);
    dz = dz * (length / currLength);

    return glm::vec3({a.x + dx, a.y + dy, a.z + dz});
}

void pushDefaultValues(std::vector<float> &vertices, int count)
{
    for (int i = 0; i < count; ++i)
    {
        vertices.push_back(0.0f);
    }
}

std::vector<float> genOctohedron()
{
    std::vector<float> vertices = {
        // top-north-east
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 0.0, 0.0,

        // top-north-west
        0.0, 1.0, 0.0,
        -1.0, 0.0, 0.0,
        0.0, 0.0, 1.0,

        // top-south-west
        0.0, 1.0, 0.0,
        0.0, 0.0, -1.0,
        -1.0, 0.0, 0.0,

        // top-south-east
        0.0, 1.0, 0.0,
        1.0, 0.0, 0.0,
        0.0, 0.0, -1.0,

        // bottom-north-east
        0.0, -1.0, 0.0,
        1.0, 0.0, 0.0,
        0.0, 0.0, 1.0,

        // bottom-north-west
        0.0, -1.0, 0.0,
        0.0, 0.0, 1.0,
        -1.0, 0.0, 0.0,

        // bottom-south-west
        0.0, -1.0, 0.0,
        -1.0, 0.0, 0.0,
        0.0, 0.0, -1.0,

        // bottom-south-east
        0.0, -1.0, 0.0,
        0.0, 0.0, -1.0,
        1.0, 0.0, 0.0,

        // hereeee
    };

    return vertices;
}

glm::vec3 midpoint(const glm::vec3 &v1, const glm::vec3 &v2)
{
    return {(v1[0] + v2[0]) / 2.0f, (v1[1] + v2[1]) / 2.0f, (v1[2] + v2[2]) / 2.0f};
}

std::vector<float> subdivide(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3, int depth)
{
    std::cout << "depth: " << depth << '\n';
    if (depth == 0)
    {
        return {
            v1[0], v1[1], v1[2],
            v2[0], v2[1], v2[2],
            v3[0], v3[1], v3[2]};
    }

    glm::vec3 m1 = glm::normalize(midpoint(v1, v2));
    glm::vec3 m2 = glm::normalize(midpoint(v2, v3));
    glm::vec3 m3 = glm::normalize(midpoint(v3, v1));

    // Recursive subdivision on each triangle
    // Subdivide the top triangle
    std::vector<float> top = subdivide(v1, m1, m3, depth - 1);
    // Subdivide the left triangle
    std::vector<float> left = subdivide(m1, v2, m2, depth - 1);
    // Subdivide the right triangle
    std::vector<float> right = subdivide(m2, v3, m3, depth - 1);
    // Subdivide the bottom triangle
    std::vector<float> bottom = subdivide(m3, m1, m2, depth - 1);

    std::vector<float> ret;
    ret.insert(ret.end(), top.begin(), top.end());
    ret.insert(ret.end(), left.begin(), left.end());
    ret.insert(ret.end(), right.begin(), right.end());
    ret.insert(ret.end(), bottom.begin(), bottom.end());

    return ret;
}

std::vector<float> genSphereFromOctahedron(int subdivisions, float radius)
{

    std::vector<float> octahedronVertices = genOctohedron();
    std::vector<float> sphereVertices;
    for (int i = 0; i < octahedronVertices.size(); i += 9)
    {
        // get triangle vertices
        const glm::vec3 v1 = {octahedronVertices[i], octahedronVertices[i + 1], octahedronVertices[i + 2]};
        const glm::vec3 v2 = {octahedronVertices[i + 3], octahedronVertices[i + 4], octahedronVertices[i + 5]};
        const glm::vec3 v3 = {octahedronVertices[i + 6], octahedronVertices[i + 7], octahedronVertices[i + 8]};

        // subdivide triangle
        std::vector<float> subdividedTriangles = subdivide(v1, v2, v3, subdivisions);
        sphereVertices.insert(sphereVertices.end(), subdividedTriangles.begin(), subdividedTriangles.end());
    }
    std::cout << sphereVertices.size() << "size \n";
    for (int i = 0; i < sphereVertices.size(); i += 3)
    {
        std::cout << sphereVertices[i] << " " << sphereVertices[i + 1] << " " << sphereVertices[i + 2] << '\n';
    }
    std::vector<float> finalVals;

    for (int i = 0; i < sphereVertices.size(); i += 3)
    {
        finalVals.push_back(sphereVertices[i] * radius);
        finalVals.push_back(sphereVertices[i + 1] * radius);
        finalVals.push_back(sphereVertices[i + 2] * radius);
        finalVals.push_back(0.0f);
        finalVals.push_back(0.0f);
        finalVals.push_back(0.0f);
        finalVals.push_back(0.0f);
        finalVals.push_back(0.0f);
    }

    return finalVals;
}

RenderComponent Factory::make_sphere_mesh(float radius)
{
    radius = 10;
    std::vector<float> vertices;
    vertices = genSphereFromOctahedron(4, 0.25f);

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
    record.vertexCount = vertices.size() / 8;
    return record;
}

unsigned int Factory::make_cube(glm::vec3 position, glm::vec3 eulers, glm::vec3 eulerVelocity, std::string texturePath, unsigned int shaderIndex)
{

    TransformComponent transform;
    transform.position = position;
    transform.eulers = eulers;
    transformComponents[entities_made] = transform;

    PhysicsComponent physics;
    physics.velocity = glm::vec3(0.0f);
    physics.eulerVelocity = eulerVelocity;
    physicsComponents[entities_made] = physics;

    RenderComponent render = make_cube_mesh(glm::vec3(1.0f));
    render.shaderIndex = shaderIndex;
    render.material = get_or_load_texture(texturePath.c_str());
    renderComponents[entities_made] = render;

    return entities_made++;
}

unsigned int Factory::make_chunk(glm::ivec2 position, std::string texturePath, unsigned int shaderIndex)
{

    // TransformComponent transform;
    // transform.position = glm::vec3(position.x, position.y, 0.0f);
    // transformComponents[entities_made] = transform;

    RenderComponent render = make_chunk_mesh(position);
    render.shaderIndex = shaderIndex;
    render.material = get_or_load_texture(texturePath.c_str());
    renderComponents[entities_made] = render;

    return entities_made++;
}
unsigned int Factory::get_or_load_texture(const char *filename)
{
    // try to find texture ID if its been loaded, if so return it
    auto cacheEntry = textureCache.find(filename);
    if (cacheEntry != textureCache.end())
        return cacheEntry->second;

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);

    textureCache[filename] = texture;
    return texture;
}

RenderComponent Factory::make_chunk_mesh(glm::ivec2 location)
{
    // std::cout << location.x << " " << location.y << "\n";
    Chunk chunk{location};
    gen.generateTerrainFor(chunk);
    chunk.buildMesh();

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * chunk.vertices.size(), chunk.vertices.data(), GL_STATIC_DRAW);

    // EBO (Element buffer object) for indices
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    EBOs.push_back(EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * chunk.indices.size(), chunk.indices.data(), GL_STATIC_DRAW);

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
    record.EBO = EBO;
    record.vertexCount = chunk.indices.size();
    return record;
}

RenderComponent Factory::make_cube_mesh(glm::vec3 size)
{
    std::vector<float> vertices = {
        // bottom face
        0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1, 0, 0, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0, 1, 0, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        1, 1, 0, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        // top face
        0, 0, 1, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        1, 0, 1, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0, 1, 1, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1, 1, 1, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        // back face
        0, 0, 0, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0, 1, 0, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0, 0, 1, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0, 1, 1, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        // front face
        1, 0, 0, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1, 1, 0, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1, 0, 1, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1, 1, 1, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        // left face
        0, 0, 0, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1, 0, 0, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        0, 0, 1, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        1, 0, 1, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        // rig1t face
        0, 1, 0, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1, 1, 0, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0, 1, 1, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1, 1, 1, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};

    std::vector<unsigned int> indices = {
        0, 1, 3, 0, 2, 3,       // bottom face
        4, 5, 7, 4, 6, 7,       // top face
        8, 9, 11, 8, 10, 11,    // back face
        12, 13, 15, 12, 14, 15, // front face
        16, 17, 19, 16, 18, 19, // left face
        20, 21, 23, 20, 22, 23  // right face
        //
    };

    // std::cout << "total length " << chunk.vertices.size() / 32 << '\n';

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

    // EBO (Element buffer objecct) for indices
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    EBOs.push_back(EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

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
    record.EBO = EBO;
    record.vertexCount = indices.size();
    return record;
}