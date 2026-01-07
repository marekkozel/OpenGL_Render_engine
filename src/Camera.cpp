#include "Camera.h"
#include "Controls.h"
#include "Transformation_manager.h"
#include "GL_common.h"

#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : position(position),
      world_up(up),
      yaw(yaw),
      pitch(pitch),
      movement_speed(2.5f),
      mouse_sensitivity(0.5f),
      front(glm::vec3(0.0f, 0.0f, -1.0f)),
      view_matrix(glm::mat4(1.0f)),
      projection_matrix(glm::mat4(1.0f))
{
    this->visible = false;
    update_camera_vectors(nullptr);
}

void Camera::update(GLFWwindow *window, float delta_time, Controls *controls)
{
    process_keyboard(window, delta_time, controls);

    float dx = (float)controls->get_mouse_delta_x();
    float dy = (float)controls->get_mouse_delta_y();

    if (controls->is_mouse_button_pressed(1))
    {
        process_mouse(dx, dy);
    }

    read_stencil_buffer((float)controls->get_mouse_x(), (float)controls->get_mouse_y(), window);

    update_camera_vectors(window);

    glm::mat4 camera_world_matrix = glm::inverse(view_matrix);

    if (transform_manager)
    {
        transform_manager->override_final_matrix(camera_world_matrix);
    }

    notify_observers();
}

void Camera::process_keyboard(GLFWwindow *window, float delta_time, Controls *controls)
{
    float velocity = movement_speed * delta_time;

    if (controls->is_key_pressed(GLFW_KEY_W))
        position += front * velocity;
    if (controls->is_key_pressed(GLFW_KEY_S))
        position -= front * velocity;
    if (controls->is_key_pressed(GLFW_KEY_A))
        position -= right * velocity;
    if (controls->is_key_pressed(GLFW_KEY_D))
        position += right * velocity;
    if (controls->is_key_pressed(GLFW_KEY_SPACE))
        position += world_up * velocity;
    if (controls->is_key_pressed(GLFW_KEY_LEFT_CONTROL))
        position -= world_up * velocity;
}

void Camera::process_mouse(double x_offset, double y_offset)
{
    x_offset *= mouse_sensitivity;
    y_offset *= mouse_sensitivity;

    yaw += (float)x_offset;
    pitch += (float)y_offset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
}

void Camera::update_camera_vectors(GLFWwindow *window)
{
    // Front
    glm::vec3 new_front;
    new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    new_front.y = sin(glm::radians(pitch));
    new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(new_front);

    // Right and Up
    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));

    view_matrix = glm::lookAt(position, position + front, up);

    int width = 800, height = 600;
    if (window)
    {
        glfwGetWindowSize(window, &width, &height);
        if (height == 0)
            height = 1;
    }

    projection_matrix = glm::perspective(glm::radians(70.0f), (float)width / (float)height, 0.1f, 100.0f);
}

void Camera::read_stencil_buffer(float dx, float dy, GLFWwindow *window)
{
    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);

    GLbyte color[4];
    GLfloat depth;
    GLuint index;

    GLint x = (GLint)dx;
    GLint y = (GLint)dy;

    int newy = window_height - y;

    glReadPixels(x, newy, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
    glReadPixels(x, newy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    glReadPixels(x, newy, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

    glm::vec3 pos(0.0f);

    if (depth != 1.0f)
    {
        glm::vec3 screenX = glm::vec3(x, newy, depth);
        glm::vec4 viewPort = glm::vec4(0, 0, window_width, window_height);
        pos = glm::unProject(screenX, view_matrix, projection_matrix, viewPort);
    }

    Controls::set_global_mouse_pos(pos);
    Controls::set_object_id((int)index);

    // Debug
    // std::cout << "Picked ID: " << index << " at World Pos: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
}

// Getters
glm::mat4 Camera::get_view_matrix() const { return view_matrix; }
glm::mat4 Camera::get_projection_matrix() const { return projection_matrix; }
float Camera::get_last_frame() const { return last_frame; }
SubjectType Camera::get_type() const { return SubjectType::Camera; }

glm::vec3 Camera::get_position() const { return position; }

// Setters
void Camera::set_last_frame(float lf) { last_frame = lf; }
void Camera::set_position(const glm::vec3 &pos) { position = pos; }
