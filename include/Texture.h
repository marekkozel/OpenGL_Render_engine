#pragma once

#include "GL_common.h"
#include <vector>
#include <string>

class Texture
{
private:
    GLuint id = 0;
    int width = 0;
    int height = 0;
    int channels = 0;

public:
    Texture() = default;

    ~Texture();

    bool load_from_file(const std::string &path);
    bool load_from_files(const std::vector<std::string> &paths);

    // Binding
    void bind(GLuint unit = 0) const;
    void bind_cubemap(GLuint unit = 0) const;
    void unbind(GLuint unit = 0) const;

    GLuint get_id() const;
};