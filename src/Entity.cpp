#include "Entity.h"
#include "Model.h"
#include "Texture.h"
#include "Material.h"
#include "Shader_program.h"
#include "Transformation_manager.h"
#include "Controls.h"
#include "Resource_manager.h"

int Entity::increment_id = 0;

Entity::Entity()
    : transform_manager(std::make_shared<Transform_manager>()),
      material(std::make_shared<Material>())
{
}

Entity::~Entity()
{
    model.reset();
    shaders.clear();
    transform_manager.reset();
    texture.reset();
    material.reset();
}

std::shared_ptr<Entity> Entity::clone() const
{
    if (increment_id++ >= 254)
    {
        increment_id = 0;
    }

    auto new_entity = std::make_shared<Entity>();

    // Copy attributes
    new_entity->color = this->color;
    new_entity->visible = this->visible;
    new_entity->id = this->id + increment_id;
    new_entity->time_to_live = this->time_to_live;
    new_entity->model = this->model;
    new_entity->shaders = this->shaders;
    new_entity->texture = this->texture;
    new_entity->material = this->material;
    new_entity->observers = this->observers;

    // Deep copy the Transform Manager
    if (this->transform_manager)
    {
        new_entity->transform_manager = this->transform_manager->clone();
    }

    return new_entity;
}

void Entity::clear()
{
    for (auto &shader : shaders)
    {
        if (shader)
            shader->clear();
    }
}

void Entity::update(GLFWwindow *window, float delta_time, Controls *controls)
{
    if (transform_manager)
    {
        transform_manager->calculate_transformations();
    }
    notify_observers();
}

void Entity::draw()
{
    if (!model || !visible || shaders.empty())
        return;

    if (texture)
    {
        texture->bind(0);
    }

    for (auto &shader : shaders)
    {
        if (shader)
        {
            shader->use(transform_manager->get_final_matrix(), color);
        }
    }

    model->draw();

    if (texture)
    {
        texture->unbind(0);
    }
}

// Setters
void Entity::set_model(std::shared_ptr<Model> m) { model = std::move(m); }
void Entity::add_shader(std::shared_ptr<Shader_program> s)
{
    shaders.push_back(std::move(s));
    // Automatically attach the shader as an observer to this entity
    this->attach_observer(shaders.back());
}
void Entity::set_transform_manager(std::shared_ptr<Transform_manager> tm) { transform_manager = std::move(tm); }
void Entity::set_visible(bool v) { visible = v; }
void Entity::set_color(const glm::vec3 &c) { color = c; }
void Entity::set_material(std::shared_ptr<Material> mat) { material = mat; }

void Entity::set_texture(std::shared_ptr<Texture> tex)
{
    texture = tex;
    notify_observers();
}

bool Entity::load_from_file(const std::string &path)
{
    auto loaded_model = Resource_manager::instance().load_model(path);
    if (loaded_model)
    {
        this->model = loaded_model;
        return true;
    }
    return false;
}

void Entity::set_id(int new_id) { id = new_id; }
void Entity::set_time_to_live(float ttl) { time_to_live = ttl; }
void Entity::set_current_time(float ct) { current_time = ct; }

// Getters
std::shared_ptr<Model> Entity::get_model() const { return model; }
std::vector<std::shared_ptr<Shader_program>> Entity::get_shaders() const { return shaders; }
std::shared_ptr<Transform_manager> Entity::get_transform_manager() const { return transform_manager; }
std::shared_ptr<Material> Entity::get_material() const { return material; }
std::shared_ptr<Texture> Entity::get_texture() const { return texture; }
bool Entity::is_visible() const { return visible; }
glm::vec3 Entity::get_color() const { return color; }
int Entity::get_id() const { return id; }
float Entity::get_time_to_live() const { return time_to_live; }
float Entity::get_current_time() const { return current_time; }

SubjectType Entity::get_type() const
{
    return SubjectType::Entity;
}

// Transformations

void Entity::add_static_transform(Transform *t)
{
    if (transform_manager)
        transform_manager->add_static_transform(t);
}

void Entity::add_local_transform(Transform *t)
{
    if (transform_manager)
        transform_manager->add_local_transform(t);
}

void Entity::add_global_transform(Transform *t)
{
    if (transform_manager)
        transform_manager->add_global_transform(t);
}

void Entity::add_parent(std::shared_ptr<Transform_manager> parent_manager, TransformParentType type)
{
    if (transform_manager)
    {
        transform_manager->set_parent_type(type);
        transform_manager->add_parent(parent_manager);
    }
}