#pragma once

#include "GL_common.h"

using namespace glm;

class Transform
{
public:
    Transform() = default;
    virtual ~Transform() = default;

    // Compute final model matrix
    virtual mat4 get_model_matrix() { return mat4(1.0f); }

    virtual Transform *clone() const = 0;
};