#pragma once

#include "GL_common.h"
#include "Observer.h"
#include <string>
#include <memory>

class Camera;
class Light;
class Texture;
class Material;
class Model;
class Subject;

class Shader_program : public Observer
{
private:
    GLuint shader_program_id;

    GLuint compile_shader(const std::string &source, GLenum type);
    std::string load_shader_source(const std::string &path);
    void create_shader(GLenum shader_type, const char *shader_code);

public:
    Shader_program();
    ~Shader_program();

    void use();
    void use(glm::mat4 model_matrix, glm::vec3 object_color);
    void clear();

    // Uses Resource_manager to load shaders
    bool load_from_files(const std::string &vertex_path, const std::string &fragment_path);

    void load_shader_from_file(GLenum shader_type, const char *shader_path);

    // Observer
    void notify(std::shared_ptr<Subject> subject) override;

    // Specific entity updates
    void camera_updated(std::shared_ptr<Camera> camera);
    void light_updated(std::shared_ptr<Light> light);
    void texture_updated(std::shared_ptr<Texture> texture);
    void material_updated(std::shared_ptr<Material> material);

    // Uniforms
    GLint get_uniform_location(std::string name);

    void set_uniform(std::shared_ptr<Camera> camera);
    void set_uniform(std::shared_ptr<Light> light);
    void set_uniform(std::shared_ptr<Material> material);

    void set_uniform(glm::mat4 m);
    void set_uniform(glm::mat4 m, glm::vec3 object_color);
};