#pragma once

#include "Transformation.h"
#include <vector>

using namespace glm;

class Bezier_transform : public Transform
{
private:
    std::vector<vec3> control_points;
    float current_t;
    float speed_t;

    float delta_time = 0.0f;
    float current_time = 0.0f;
    float last_time = 0.0f;

    mat4 last_matrix;
    bool first_update = true;

public:
    Bezier_transform(const std::vector<vec3> &points, float speed = 0.2f);

    mat4 get_model_matrix() override;
    Transform *clone() const override;
};