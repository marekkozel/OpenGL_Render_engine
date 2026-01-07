#pragma once
#include "Entity.h"
#include "GL_common.h"
#include <vector>
#include <memory>

class Model;
class Shader_program;

class Drawable_object : public Entity
{
public:
    Drawable_object();

    Drawable_object(std::shared_ptr<Model> model,
                    std::shared_ptr<Shader_program> shader,
                    glm::vec3 color = glm::vec3(1.0f, 0.45f, 0.0f));

    Drawable_object(std::shared_ptr<Model> model,
                    std::vector<std::shared_ptr<Shader_program>> shaders,
                    glm::vec3 color = glm::vec3(1.0f, 0.45f, 0.0f));

    ~Drawable_object() override;

    void update(GLFWwindow *window, float delta_time, Controls *controls) override;

    SubjectType get_type() const override;
};