#pragma once

#include "Subject.h"
#include <memory>
#include <vector>
#include <string>
#include "GL_common.h"
#include "Transformation_manager.h"

class Model;
class Texture;
class Material;
class Transform_manager;
class Transform;
class Shader_program;
class Controls;
enum class TransformParentType;

class Entity : public Subject
{
protected:
    int id = -1;
    static int increment_id;
    bool visible = true;
    float time_to_live = -1.0f;
    float current_time = 0.0f;

    glm::vec3 color = glm::vec3(1.0f, 0.45f, 0.0f);

    std::shared_ptr<Model> model;
    std::vector<std::shared_ptr<Shader_program>> shaders;
    std::shared_ptr<Transform_manager> transform_manager;
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Material> material;

public:
    Entity();
    virtual ~Entity();

    virtual std::shared_ptr<Entity> clone() const;

    void clear();
    virtual void update(GLFWwindow *window, float delta_time, Controls *controls);
    void draw();

    // Setters
    void set_model(std::shared_ptr<Model> m);
    void add_shader(std::shared_ptr<Shader_program> s);
    void set_transform_manager(std::shared_ptr<Transform_manager> tm);
    void set_visible(bool v);
    void set_color(const glm::vec3 &c);
    void set_material(std::shared_ptr<Material> mat);
    void set_texture(std::shared_ptr<Texture> tex);
    void set_current_time(float ct);
    void set_id(int new_id);
    void set_time_to_live(float ttl);

    // Uses Resource_manager to load model
    bool load_from_file(const std::string &path);

    // Getters
    std::shared_ptr<Model> get_model() const;
    std::vector<std::shared_ptr<Shader_program>> get_shaders() const;
    std::shared_ptr<Transform_manager> get_transform_manager() const;
    std::shared_ptr<Material> get_material() const;
    std::shared_ptr<Texture> get_texture() const;

    bool is_visible() const;
    glm::vec3 get_color() const;
    int get_id() const;
    float get_time_to_live() const;
    float get_current_time() const;

    SubjectType get_type() const override;

    // Transform Helpers
    void add_static_transform(Transform *t);
    void add_local_transform(Transform *t);
    void add_global_transform(Transform *t);

    void add_parent(std::shared_ptr<Transform_manager> parent_manager, TransformParentType type = TransformParentType::Everything);
};