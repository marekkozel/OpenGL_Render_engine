#pragma once

#include "Transformation.h"
#include <vector>
#include <utility>

using namespace glm;

class Timer_transform : public Transform
{
private:
    float delta_time = 0.0f;
    float current_time = 0.0f;
    float last_time = 0.0f;

    // pair< duration, transform_ptr >, current_elapsed
    std::vector<std::pair<std::pair<float, Transform *>, float>> timer_transforms;

public:
    Timer_transform(std::vector<std::pair<float, Transform *>> transforms = {});

    void add_transformation(float timer, Transform *transformation);

    mat4 get_model_matrix() override;
    Transform *clone() const override;
};