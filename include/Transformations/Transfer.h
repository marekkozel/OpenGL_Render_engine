#pragma once

#include "Transformation.h"

using namespace glm;

class Transfer : public Transform
{
private:
    vec3 basic_vector;
    vec3 random_min;
    vec3 random_max;
    bool is_random = false;

    void randomize();

public:
    Transfer(vec3 vec);
    Transfer(vec3 min_range, vec3 max_range);

    mat4 get_model_matrix() override;
    Transform *clone() const override;
};