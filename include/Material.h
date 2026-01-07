#pragma once

#include "GL_common.h"

class Material
{
private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

public:
    Material(const glm::vec3 &ambient = glm::vec3(0.1f),
             const glm::vec3 &diffuse = glm::vec3(1.0f),
             const glm::vec3 &specular = glm::vec3(0.5f),
             float shininess = 32.0f);

    // Getters
    glm::vec3 get_ambient() const;
    glm::vec3 get_diffuse() const;
    glm::vec3 get_specular() const;
    float get_shininess() const;

    // Setters
    void set_ambient(const glm::vec3 &a);
    void set_diffuse(const glm::vec3 &d);
    void set_specular(const glm::vec3 &s);
    void set_shininess(float h);
};