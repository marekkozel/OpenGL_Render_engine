#pragma once

#include "GL_common.h"
#include "Entity.h"

class Model;
class Shader_program;
class Controls;

enum class LightType
{
    Ambient = 0,
    Directional = 1,
    Point = 2,
    Reflector = 3
};

class Light : public Entity
{
private:
    static int light_count;
    int id;
    float intensity;
    float distance = 1.0f;
    LightType type;

public:
    Light();

    // Constructor for invisible lights (just a source of light)
    Light(glm::vec3 color, LightType type = LightType::Point, float intensity = 1.0f, float distance = 1.0f);

    // Constructor for visible lights (has a 3D model representation)
    Light(std::shared_ptr<Model> model,
          std::shared_ptr<Shader_program> shader,
          glm::vec3 color,
          LightType type = LightType::Point,
          float distance = 1.0f);

    ~Light() override;

    // === Lifecycle ===
    void update(GLFWwindow *window, float delta_time, Controls *controls) override;
    void process_keyboard(GLFWwindow *window, float delta_time, Controls *controls);

    // === Getters & Setters ===
    int get_id() const;
    int get_count() const;

    float get_intensity() const;
    void set_intensity(float i);

    float get_distance() const;
    void set_distance(float distance);

    LightType get_light_type() const;

    SubjectType get_type() const override;
};