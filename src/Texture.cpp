#include "Texture.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::~Texture()
{
    if (id != 0)
    {
        glDeleteTextures(1, &id);
        id = 0;
    }
}

bool Texture::load_from_file(const std::string &path)
{

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 4);

    if (!data)
    {
        std::cerr << "❌ Failed to load texture: " << path << std::endl;
        return false;
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return true;
}

bool Texture::load_from_files(const std::vector<std::string> &paths)
{
    if (paths.size() != 6)
    {
        std::cerr << "❌ Error: Skybox requires 6 textures." << std::endl;
        return false;
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    int w, h, c;
    stbi_set_flip_vertically_on_load(false);

    for (unsigned int i = 0; i < 6; i++)
    {
        unsigned char *data = stbi_load(paths[i].c_str(), &w, &h, &c, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cerr << "❌ Failed to load cubemap face: " << paths[i] << std::endl;
            stbi_image_free(data);
            return false;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return true;
}

void Texture::bind(GLuint unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::bind_cubemap(GLuint unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

void Texture::unbind(GLuint unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::get_id() const
{
    return id;
}