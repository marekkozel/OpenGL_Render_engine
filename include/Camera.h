#pragma once

#include "GL_common.h"
#include "Entity.h"

class GLFWwindow;
class Controls;

class Camera : public Entity
{
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;

    float yaw;
    float pitch;

    float movement_speed;
    float mouse_sensitivity;

    float last_frame = 0.0f;

    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;

public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f,
           float pitch = 0.0f);

    void update(GLFWwindow *window, float delta_time, Controls *controls) override;

    void process_keyboard(GLFWwindow *window, float delta_time, Controls *controls);
    void process_mouse(double x_offset, double y_offset);

    void update_camera_vectors(GLFWwindow *window);

    void read_stencil_buffer(float dx, float dy, GLFWwindow *window);

    glm::mat4 get_view_matrix() const;
    glm::mat4 get_projection_matrix() const;
    glm::vec3 get_position() const;
    void set_position(const glm::vec3 &pos);

    void set_last_frame(float lf);
    float get_last_frame() const;

    SubjectType get_type() const override;
};