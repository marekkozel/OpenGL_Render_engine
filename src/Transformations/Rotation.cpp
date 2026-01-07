#include "Rotation.h"
#include <cstdlib>

Rotation::Rotation(glm::vec3 vec, float angle)
    : rotation_angle(angle), basic_vector(vec), is_random(false) {}

Rotation::Rotation(glm::vec3 min_range, glm::vec3 max_range)
    : basic_vector(glm::vec3(0.0f)), random_min(min_range), random_max(max_range), is_random(true) {}

void Rotation::randomize()
{
    if (is_random)
    {
        basic_vector.x = random_min.x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (random_max.x - random_min.x)));
        basic_vector.y = random_min.y + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (random_max.y - random_min.y)));
        basic_vector.z = random_min.z + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (random_max.z - random_min.z)));
    }
}

glm::mat4 Rotation::get_model_matrix()
{
    glm::mat4 model(1.0f);
    randomize();

    if (rotation_angle == 0.0f)
    {
        model = glm::rotate(model, basic_vector.z, glm::vec3(0, 0, 1));
        model = glm::rotate(model, basic_vector.y, glm::vec3(0, 1, 0));
        model = glm::rotate(model, basic_vector.x, glm::vec3(1, 0, 0));
    }
    else
    {
        model = glm::rotate(model, rotation_angle, basic_vector);
    }

    return model;
}

Transform *Rotation::clone() const
{
    return new Rotation(*this);
}