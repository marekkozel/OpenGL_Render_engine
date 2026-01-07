#include "Parametric_transformation.h"
#include <iostream>
#include <cmath>
#include "GL_common.h"

Parametric_transform::Parametric_transform(const vector<vec3> &points, float speed)
    : control_points(points), current_t(0.0f), speed_t(speed)
{
    if (control_points.size() < 2)
    {
        control_points = {vec3(0.0f), vec3(1.0f)};
    }
    last_position = control_points[0];
}

mat4 Parametric_transform::get_model_matrix()
{
    mat4 model = mat4(1.0f);

    current_time = (float)glfwGetTime();
    delta_time = current_time - last_time;
    last_time = current_time;

    current_t += speed_t * delta_time;

    if (current_t > 1.0f)
    {
        current_t = fmod(current_t, 1.0f);
    }

    size_t num_segments = control_points.size() - 1;
    float t_scaled = current_t * (float)num_segments;
    size_t segment_index = (size_t)floor(t_scaled);
    float u = t_scaled - (float)segment_index;

    if (segment_index >= num_segments)
    {
        segment_index = num_segments - 1;
        u = 1.0f;
    }

    vec3 P_i = control_points[segment_index];
    vec3 P_i_plus_1 = control_points[segment_index + 1];

    // Linear interpolation for current position
    vec3 current_position = mix(P_i, P_i_plus_1, u);

    if (first_update)
    {
        last_position = current_position;
        first_update = false;
        return model;
    }

    // Calculate delta
    vec3 delta = current_position - last_position;

    last_position = current_position;

    return translate(model, delta);
}

Transform *Parametric_transform::clone() const
{
    return new Parametric_transform(*this);
}