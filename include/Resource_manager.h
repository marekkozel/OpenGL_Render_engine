#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include "Model.h"
#include "Shader_program.h"
#include "Texture.h"

class Resource_manager
{
private:
    std::unordered_map<std::string, std::shared_ptr<Model>> models;
    std::unordered_map<std::string, std::shared_ptr<Shader_program>> shaders;
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

    // Private constructor for Singleton
    Resource_manager() = default;

public:
    // Singleton Accessor
    static Resource_manager &instance();

    // Prevent copying and assignment
    Resource_manager(const Resource_manager &) = delete;
    Resource_manager &operator=(const Resource_manager &) = delete;

    // Models
    std::shared_ptr<Model> load_model(const std::string &path);
    std::shared_ptr<Model> get_model(const std::string &path);

    // Shaders
    std::shared_ptr<Shader_program> load_shader(const std::string &vert, const std::string &frag);
    std::shared_ptr<Shader_program> get_shader(const std::string &vert, const std::string &frag);

    // Textures
    std::shared_ptr<Texture> load_texture(const std::string &path);
    // Load a cubemap texture (skybox)
    std::shared_ptr<Texture> load_cubemap(const std::vector<std::string> &paths);

    void clear();
};