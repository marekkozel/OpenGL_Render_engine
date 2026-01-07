#include "Drawable_object.h"
#include "Model.h"
#include "Shader_program.h"
#include "Transformation_manager.h"

Drawable_object::Drawable_object()
{
    // In entity
}

Drawable_object::~Drawable_object()
{
    // In entity
}

Drawable_object::Drawable_object(std::shared_ptr<Model> model,
                                 std::shared_ptr<Shader_program> shader,
                                 glm::vec3 color)
{
    this->model = model;
    this->color = color;

    this->add_shader(shader);
}

Drawable_object::Drawable_object(std::shared_ptr<Model> model,
                                 std::vector<std::shared_ptr<Shader_program>> shaders,
                                 glm::vec3 color)
{
    this->model = model;
    this->color = color;
    this->shaders = shaders;

    for (const auto &shader : shaders)
    {
        this->attach_observer(shader);
    }
}

void Drawable_object::update(GLFWwindow *window, float delta_time, Controls *controls)
{
    if (transform_manager)
    {
        transform_manager->calculate_transformations();
    }
    this->notify_observers();
}

SubjectType Drawable_object::get_type() const
{
    return SubjectType::DrawableObject;
}