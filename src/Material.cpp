#include "Material.h"

Material::Material(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float shininess)
{
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->shininess = shininess;
}

// Getters

glm::vec3 Material::get_ambient() const
{
    return ambient;
}

glm::vec3 Material::get_diffuse() const
{
    return diffuse;
}

glm::vec3 Material::get_specular() const
{
    return specular;
}

float Material::get_shininess() const
{
    return shininess;
}

// Setters

void Material::set_ambient(const glm::vec3 &a)
{
    ambient = a;
}

void Material::set_diffuse(const glm::vec3 &d)
{
    diffuse = d;
}

void Material::set_specular(const glm::vec3 &s)
{
    specular = s;
}

void Material::set_shininess(float h)
{
    shininess = h;
}