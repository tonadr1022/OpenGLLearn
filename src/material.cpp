#include "material.h"
#include "stb_image/stb_image.h"

// Material::Material(const char *filename)
// {
//     // load data from the file
//     int width, height, channels;

//     stbi_set_flip_vertically_on_load(true);

//     unsigned char *data = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);
//     if (data)
//     {
//         // make the texture
//         glGenTextures(1, &texture);
//         glBindTexture(GL_TEXTURE_2D, texture);

//         // upload to GPU, describe layout on the GPU (mipmap, etc), then define data coming in (format, type, data)
//         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//         glGenerateMipmap(GL_TEXTURE_2D);
//     }
//     else
//     {
//         std::cout << "Failed to load texture" << std::endl;
//     }

//     // free data (it's already on the GPU now)
//     stbi_image_free(data);

//     // configure sampler

//     // s = x-axis
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     // t = y-axis
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     // how to sample when image is shrunk down?
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//     // how to sample when image is blown up?
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// }

// Material::~Material()
// {
//     glDeleteTextures(1, &texture);
// }

// void Material::use(int unit)
// {
//     glActiveTexture(GL_TEXTURE0 + unit);
//     glBindTexture(GL_TEXTURE_2D, texture);
// }