#include "Controls.h"

std::unordered_map<int, bool> Controls::keys;
std::unordered_map<int, bool> Controls::mouse_buttons;
std::unordered_map<int, bool> Controls::last_keys;
std::unordered_map<int, bool> Controls::last_mouse_buttons;

double Controls::mouse_x = 0.0;
double Controls::mouse_y = 0.0;
double Controls::last_mouse_x = 0.0;
double Controls::last_mouse_y = 0.0;
double Controls::mouse_delta_x = 0.0;
double Controls::mouse_delta_y = 0.0;
bool Controls::first_mouse = true;

glm::vec3 Controls::global_mouse_pos = glm::vec3(0.0f);
int Controls::object_id = -1;

Controls::Controls() {}

void Controls::attach_to_window(GLFWwindow *window)
{
    glfwSetErrorCallback(Controls::error_callback);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    glfwSetWindowUserPointer(window, this);
}

bool Controls::is_key_pressed(int key) const
{
    auto it = keys.find(key);
    return it != keys.end() && it->second;
}

bool Controls::is_key_released(int key) const
{
    auto it = keys.find(key);
    return it == keys.end() || !it->second;
}

bool Controls::is_key_triggered(int key) const
{
    bool key_down = is_key_pressed(key);
    bool was_down = (last_keys.find(key) != last_keys.end() && last_keys.at(key));

    return key_down && !was_down;
}

bool Controls::is_mouse_button_pressed(int button) const
{
    auto it = mouse_buttons.find(button);
    return it != mouse_buttons.end() && it->second;
}

bool Controls::is_mouse_button_triggered(int button) const
{
    bool is_down = is_mouse_button_pressed(button);
    bool was_down = (last_mouse_buttons.find(button) != last_mouse_buttons.end() && last_mouse_buttons.at(button));

    return is_down && !was_down;
}

// Callbacks
void Controls::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}

void Controls::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
        mouse_buttons[button] = true;
    else if (action == GLFW_RELEASE)
        mouse_buttons[button] = false;
}

void Controls::cursor_pos_callback(GLFWwindow *window, double xpos, double ypos)
{
    mouse_x = xpos;
    mouse_y = ypos;

    if (first_mouse)
    {
        last_mouse_x = xpos;
        last_mouse_y = ypos;
        first_mouse = false;
    }

    mouse_delta_x = xpos - last_mouse_x;
    mouse_delta_y = last_mouse_y - ypos;

    last_mouse_x = xpos;
    last_mouse_y = ypos;
}

void Controls::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Controls::error_callback(int error, const char *description)
{
    std::cerr << "❌ GLFW Error: " << description << std::endl;
}

void Controls::end_frame()
{
    last_keys = keys;
    last_mouse_buttons = mouse_buttons;

    reset_mouse_delta();
}

void Controls::reset_mouse_delta()
{
    mouse_delta_x = 0.0;
    mouse_delta_y = 0.0;
}