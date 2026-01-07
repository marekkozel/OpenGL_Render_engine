#pragma once

#include "Transformation.h"
#include <vector>

using namespace glm;
using namespace std;

class Parametric_transform : public Transform
{
private:
    vector<vec3> control_points;
    float current_t;
    float speed_t;

    float delta_time = 0.0f;
    float current_time = 0.0f;
    float last_time = 0.0f;

    vec3 last_position;
    bool first_update = true;

public:
    Parametric_transform(const vector<vec3> &points, float speed = 0.1f);

    mat4 get_model_matrix() override;
    Transform *clone() const override;
};