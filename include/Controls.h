#pragma once

#include "GL_common.h"
#include <iostream>
#include <unordered_map>

class Controls
{
private:
    // GLFW Callbacks (must be static)
    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    static void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos);
    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
    static void error_callback(int error, const char *description);

    // State Storage
    static std::unordered_map<int, bool> keys;
    static std::unordered_map<int, bool> mouse_buttons;
    static std::unordered_map<int, bool> last_keys;
    static std::unordered_map<int, bool> last_mouse_buttons;

    // Mouse Data
    static double mouse_x;
    static double mouse_y;
    static double last_mouse_x;
    static double last_mouse_y;
    static double mouse_delta_x;
    static double mouse_delta_y;
    static bool first_mouse;

    // 3D Cursor
    static glm::vec3 global_mouse_pos;
    static int object_id;

public:
    Controls();

    // Setup
    void attach_to_window(GLFWwindow *window);

    // Input
    bool is_key_pressed(int key) const;
    bool is_key_released(int key) const;
    bool is_key_triggered(int key) const;

    bool is_mouse_button_pressed(int button) const;
    bool is_mouse_button_triggered(int button) const;

    // Mouse Getters
    double get_mouse_x() const { return mouse_x; }
    double get_mouse_y() const { return mouse_y; }
    double get_mouse_delta_x() const { return mouse_delta_x; }
    double get_mouse_delta_y() const { return mouse_delta_y; }

    // Frame
    static void end_frame();
    static void reset_mouse_delta();

    // Getters/Setters
    static glm::vec3 get_global_mouse_pos() { return global_mouse_pos; }
    static void set_global_mouse_pos(const glm::vec3 &pos) { global_mouse_pos = pos; }

    static int get_object_id() { return object_id; }
    static void set_object_id(int id) { object_id = id; }
};