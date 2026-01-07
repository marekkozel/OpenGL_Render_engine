#include "Light.h"
#include "Controls.h"
#include "Model.h"
#include "Shader_program.h"
#include "Transformation_manager.h"

int Light::light_count = 0;

Light::Light()
{
    id = light_count++;
    this->color = glm::vec3(1.0f, 1.0f, 1.0f);
    this->intensity = 1.0f;
    this->type = LightType::Point;
    this->distance = 1.0f;
}

Light::Light(glm::vec3 color, LightType type, float intensity, float distance)
{
    id = light_count++;
    this->color = color;
    this->intensity = intensity;
    this->type = type;
    this->distance = distance;
}

Light::Light(std::shared_ptr<Model> model,
             std::shared_ptr<Shader_program> shader,
             glm::vec3 color,
             LightType type,
             float distance)
{
    this->model = model;
    this->shaders.push_back(shader);
    // this->attach_observer(shader);

    id = light_count++;
    this->color = color;
    this->type = type;
    this->distance = distance;
    this->intensity = 1.0f;
}

Light::~Light()
{
    light_count--;
}

void Light::update(GLFWwindow *window, float delta_time, Controls *controls)
{
    process_keyboard(window, delta_time, controls);

    if (transform_manager)
    {
        transform_manager->calculate_transformations();
    }

    notify_observers();
}

void Light::process_keyboard(GLFWwindow *window, float delta_time, Controls *controls)
{
    // Flashlight toggle
    if (controls->is_key_triggered(GLFW_KEY_F) && this->type == LightType::Reflector)
    {
        if (intensity > 0.0f)
            intensity = 0.0f;
        else
            intensity = 1.0f;

        notify_observers();
    }
}

// Getters
int Light::get_id() const { return id; }
int Light::get_count() const { return light_count; }

float Light::get_intensity() const { return intensity; }

float Light::get_distance() const { return distance; }

LightType Light::get_light_type() const { return type; }

SubjectType Light::get_type() const
{
    return SubjectType::Light;
}

// Setters
void Light::set_intensity(float i) { intensity = i; }
void Light::set_distance(float d) { distance = d; }