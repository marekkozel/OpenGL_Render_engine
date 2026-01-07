#pragma once

#include "Transformation.h"
#include <vector>

using namespace glm;

class Complex_transform : public Transform
{
private:
    std::vector<Transform *> basic_transforms;

public:
    Complex_transform(std::vector<Transform *> transforms = {});

    ~Complex_transform() override = default;

    void add_transformation(Transform *transformation);

    mat4 get_model_matrix() override;
    Transform *clone() const override;
};