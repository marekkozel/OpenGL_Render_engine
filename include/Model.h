#pragma once

#include <vector>
#include <string>
#include <memory>
#include "GL_common.h"

class Model
{
private:
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
    GLenum type = GL_TRIANGLES;

    std::shared_ptr<std::vector<float>> vertices;
    std::shared_ptr<std::vector<unsigned int>> indices;

public:
    Model(const float *vec, int size, GLenum type);
    Model(std::vector<float> *vec, GLenum type);
    Model(std::vector<float> *vec, GLenum type, int texture_stride_config);

    Model() = default;
    ~Model();

    bool load_from_file(const std::string &path);
    void draw() const;
};