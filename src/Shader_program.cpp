#include "Shader_program.h"

#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Texture.h"
#include "Entity.h"
#include "Transformation_manager.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

Shader_program::Shader_program()
{
    shader_program_id = glCreateProgram();
    glUseProgram(shader_program_id);
    glUniform1i(get_uniform_location("textureEnabled"), 0);
}

Shader_program::~Shader_program()
{
    glDeleteProgram(shader_program_id);
}

void Shader_program::use()
{
    glUseProgram(shader_program_id);
}

void Shader_program::use(glm::mat4 m, glm::vec3 object_color)
{
    glUseProgram(shader_program_id);
    this->set_uniform(m, object_color);
}

void Shader_program::clear()
{
    this->set_uniform(glm::mat4(1.0f), glm::vec3(1.0f));
    glUniform1i(get_uniform_location("textureEnabled"), 0); // Reset texture state
    glUniform1i(get_uniform_location("numLights"), 0);      // Reset light count
    glUseProgram(0);
}

// ==a= Loading

std::string Shader_program::load_shader_source(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "❌ Failed to open shader file: " << path << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint Shader_program::compile_shader(const std::string &source, GLenum type)
{
    GLuint shader_id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(shader_id, 1, &src, nullptr);
    glCompileShader(shader_id);

    GLint success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        GLint max_length = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &max_length);
        std::string info_log(max_length, ' ');
        glGetShaderInfoLog(shader_id, max_length, &max_length, &info_log[0]);

        std::cerr << "❌ Shader compilation failed ("
                  << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT")
                  << "):\n"
                  << info_log << std::endl;
        glDeleteShader(shader_id);
        return 0;
    }

    return shader_id;
}

bool Shader_program::load_from_files(const std::string &vertex_path, const std::string &fragment_path)
{
    // Clean up if re-loading
    if (shader_program_id != 0)
        glDeleteProgram(shader_program_id);

    std::string vertex_src = load_shader_source(vertex_path);
    std::string fragment_src = load_shader_source(fragment_path);

    if (vertex_src.empty() || fragment_src.empty())
        return false;

    GLuint vertex_shader = compile_shader(vertex_src, GL_VERTEX_SHADER);
    GLuint fragment_shader = compile_shader(fragment_src, GL_FRAGMENT_SHADER);

    if (!vertex_shader || !fragment_shader)
        return false;

    shader_program_id = glCreateProgram();
    glAttachShader(shader_program_id, vertex_shader);
    glAttachShader(shader_program_id, fragment_shader);
    glLinkProgram(shader_program_id);

    GLint success;
    glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);

    if (!success)
    {
        GLint max_length = 0;
        glGetProgramiv(shader_program_id, GL_INFO_LOG_LENGTH, &max_length);
        std::string info_log(max_length, ' ');
        glGetProgramInfoLog(shader_program_id, max_length, &max_length, &info_log[0]);

        std::cerr << "❌ Shader linking failed:\n"
                  << info_log << std::endl;

        glDeleteProgram(shader_program_id);
        shader_program_id = 0;

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return false;
    }

    // Shaders can be detached and deleted after linking
    glDetachShader(shader_program_id, vertex_shader);
    glDetachShader(shader_program_id, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    std::cout << "✅ Successfully loaded shaders: " << vertex_path << " & " << fragment_path << std::endl;
    return true;
}

void Shader_program::load_shader_from_file(GLenum shader_type, const char *shader_path)
{
    std::string source = load_shader_source(shader_path);
    create_shader(shader_type, source.c_str());
}

void Shader_program::create_shader(GLenum shader_type, const char *shader_code)
{
    // Helper used by the legacy/single load method
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_code, NULL);
    glCompileShader(shader);
    glAttachShader(shader_program_id, shader);
    glLinkProgram(shader_program_id);
    // Note: Error checking omitted in this legacy helper, relying on load_from_files mostly
}

// Observer Notification

void Shader_program::notify(std::shared_ptr<Subject> subject)
{
    switch (subject->get_type())
    {
    case SubjectType::Camera:
        this->camera_updated(std::static_pointer_cast<Camera>(subject));
        return;
        break;
    case SubjectType::Light:
        this->light_updated(std::static_pointer_cast<Light>(subject));
        break;
    }
    auto entity = std::static_pointer_cast<Entity>(subject);
    this->texture_updated(entity->get_texture());
    this->material_updated(entity->get_material());
}

void Shader_program::camera_updated(std::shared_ptr<Camera> camera)
{
    glUseProgram(shader_program_id);
    this->set_uniform(camera);
}

void Shader_program::light_updated(std::shared_ptr<Light> light)
{
    glUseProgram(shader_program_id);
    this->set_uniform(light);
}

void Shader_program::texture_updated(std::shared_ptr<Texture> texture)
{
    glUseProgram(shader_program_id);
    GLint tex_enabled_loc = this->get_uniform_location("textureEnabled");
    if (texture)
    {
        glUniform1i(tex_enabled_loc, 1);
        glUniform1i(get_uniform_location("textureUnitID"), 0);
    }
    else
    {
        glUniform1i(tex_enabled_loc, 0);
    }
}

void Shader_program::material_updated(std::shared_ptr<Material> material)
{
    glUseProgram(shader_program_id);
    if (material)
    {
        this->set_uniform(material);
    }
}

// Uniform Setting

GLint Shader_program::get_uniform_location(std::string name)
{
    GLint location = glGetUniformLocation(shader_program_id, name.c_str());
    // if (location == -1) std::cerr << "Warning: Uniform '" << name << "' not found." << std::endl;
    return location;
}

void Shader_program::set_uniform(std::shared_ptr<Camera> camera)
{
    glUniformMatrix4fv(get_uniform_location("viewMatrix"), 1, GL_FALSE, &camera->get_view_matrix()[0][0]);
    glUniformMatrix4fv(get_uniform_location("projectionMatrix"), 1, GL_FALSE, &camera->get_projection_matrix()[0][0]);
    glUniform3fv(get_uniform_location("viewPosition"), 1, &camera->get_position()[0]);
}

void Shader_program::set_uniform(std::shared_ptr<Light> light)
{
    std::string base = "lights[" + std::to_string(light->get_id()) + "]";

    // Reflector
    if (light->get_light_type() == LightType::Reflector)
    {
        glm::vec4 localDirection = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

        // Calculate global direction based on rotation
        glm::vec3 reflectorDirection = glm::normalize(glm::vec3(light->get_transform_manager()->get_final_matrix() * localDirection));

        glUniform3fv(get_uniform_location(base + ".direction"), 1, &reflectorDirection[0]);
    }

    // Position
    glm::mat4 lightModelMatrix = light->get_transform_manager()->get_final_matrix();
    glm::vec3 lightPosition = glm::vec3(lightModelMatrix[3]); // Transfer column

    glUniform1i(get_uniform_location(base + ".type"), static_cast<int>(light->get_light_type()));
    glUniform1f(get_uniform_location(base + ".distance"), light->get_distance());
    glUniform3fv(get_uniform_location(base + ".position"), 1, &lightPosition[0]);
    glUniform3fv(get_uniform_location(base + ".color"), 1, &light->get_color()[0]);
    glUniform1f(get_uniform_location(base + ".intensity"), light->get_intensity());

    glUniform1i(get_uniform_location("numLights"), light->get_count());
}

void Shader_program::set_uniform(glm::mat4 m, glm::vec3 object_color)
{
    glUniformMatrix4fv(get_uniform_location("modelMatrix"), 1, GL_FALSE, &m[0][0]);
    glUniform3fv(get_uniform_location("objectColor"), 1, &object_color[0]);
}

void Shader_program::set_uniform(glm::mat4 m)
{
    glUniformMatrix4fv(get_uniform_location("modelMatrix"), 1, GL_FALSE, &m[0][0]);
}

void Shader_program::set_uniform(std::shared_ptr<Material> material)
{
    if (!material)
        return;

    GLint loc_ambient = get_uniform_location("material.ambient");
    GLint loc_diffuse = get_uniform_location("material.diffuse");
    GLint loc_specular = get_uniform_location("material.specular");
    GLint loc_shininess = get_uniform_location("material.shininess");

    if (loc_ambient != -1)
        glUniform3fv(loc_ambient, 1, &material->get_ambient()[0]);
    if (loc_diffuse != -1)
        glUniform3fv(loc_diffuse, 1, &material->get_diffuse()[0]);
    if (loc_specular != -1)
        glUniform3fv(loc_specular, 1, &material->get_specular()[0]);
    if (loc_shininess != -1)
        glUniform1f(loc_shininess, material->get_shininess());
}