#include "Transfer.h"
#include <cstdlib>

Transfer::Transfer(glm::vec3 vec)
    : basic_vector(vec), is_random(false) {}

Transfer::Transfer(glm::vec3 min_range, glm::vec3 max_range)
    : basic_vector(glm::vec3(0.0f)), random_min(min_range), random_max(max_range), is_random(true) {}

void Transfer::randomize()
{
    if (is_random)
    {
        basic_vector.x = random_min.x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (random_max.x - random_min.x)));
        basic_vector.y = random_min.y + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (random_max.y - random_min.y)));
        basic_vector.z = random_min.z + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (random_max.z - random_min.z)));
    }
}

glm::mat4 Transfer::get_model_matrix()
{
    glm::mat4 model(1.0f);
    randomize();
    return glm::translate(model, basic_vector);
}

Transform *Transfer::clone() const
{
    return new Transfer(*this);
}