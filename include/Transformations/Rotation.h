#pragma once

#include "Transformation.h"

using namespace glm;

class Rotation : public Transform
{
private:
    float rotation_angle = 0.0f;
    vec3 basic_vector;
    vec3 random_min;
    vec3 random_max;
    bool is_random = false;

    void randomize();

public:
    Rotation(vec3 vec, float angle = 0.0f);
    Rotation(vec3 min_range, vec3 max_range);

    mat4 get_model_matrix() override;
    Transform *clone() const override;
};