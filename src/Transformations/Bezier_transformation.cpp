#include "Bezier_transformation.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include "GL_common.h"

Bezier_transform::Bezier_transform(const std::vector<glm::vec3> &points, float speed)
    : control_points(points), current_t(0.0f), speed_t(speed)
{

    if (control_points.size() < 4)
    {
        control_points = {
            glm::vec3(0),
            glm::vec3(0, 1, 0),
            glm::vec3(1, 1, 0),
            glm::vec3(1, 0, 0)};
    }

    last_time = (float)glfwGetTime();

    last_matrix = glm::mat4(1.0f);
}

glm::mat4 Bezier_transform::get_model_matrix()
{
    // Time Calculation
    current_time = (float)glfwGetTime();
    delta_time = current_time - last_time;
    last_time = current_time;

    current_t += speed_t * delta_time;
    if (current_t > 1.0f)
        current_t = fmod(current_t, 1.0f);

    // Determine Segment
    size_t num_segments = (control_points.size() - 1) / 3;
    if (num_segments == 0)
        num_segments = 1;

    float t_scaled = current_t * (float)num_segments;
    size_t segment_index = (size_t)floor(t_scaled);
    float u = t_scaled - (float)segment_index;

    if (segment_index >= num_segments)
    {
        segment_index = num_segments - 1;
        u = 1.0f;
    }

    size_t idx = segment_index * 3;

    if (idx + 3 >= control_points.size())
        return glm::mat4(1.0f);

    glm::vec3 p0 = control_points[idx];
    glm::vec3 p1 = control_points[idx + 1];
    glm::vec3 p2 = control_points[idx + 2];
    glm::vec3 p3 = control_points[idx + 3];

    // Calculate Position (Bernstein Polynomials)
    float uu = (1.0f - u) * (1.0f - u);
    float uuu = uu * (1.0f - u);
    float tt = u * u;
    float ttt = tt * u;

    glm::vec3 pos = (uuu * p0) + (3 * uu * u * p1) + (3 * (1.0f - u) * tt * p2) + (ttt * p3);

    // Calculate Tangent (Derivative) for Orientation
    // P'(u) = 3(1-u)^2 (P1-P0) + 6(1-u)u (P2-P1) + 3u^2 (P3-P2)
    glm::vec3 q0 = p1 - p0;
    glm::vec3 q1 = p2 - p1;
    glm::vec3 q2 = p3 - p2;

    glm::vec3 tangent = 3.0f * uu * q0 + 6.0f * (1.0f - u) * u * q1 + 3.0f * tt * q2;

    if (glm::length(tangent) < 0.001f)
        tangent = glm::vec3(0.0f, 0.0f, 1.0f);
    else
        tangent = glm::normalize(tangent);

    // Construct Absolute Model Matrix (Position + Rotation)
    glm::mat4 rotation = glm::inverse(glm::lookAt(glm::vec3(0.0f), -tangent, glm::vec3(0.0f, 1.0f, 0.0f)));

    glm::mat4 current_matrix = glm::translate(glm::mat4(1.0f), pos) * rotation;

    // Calculate Delta Matrix (Change since last frame)
    glm::mat4 delta_matrix;

    if (first_update)
    {
        delta_matrix = current_matrix;
        first_update = false;
    }
    else
    {
        delta_matrix = glm::inverse(last_matrix) * current_matrix;
    }

    last_matrix = current_matrix;

    return delta_matrix;
}

Transform *Bezier_transform::clone() const
{
    auto tmp_b = new Bezier_transform(control_points, speed_t);
    tmp_b->first_update = false;
    return tmp_b;
}