#include "Resource_manager.h"
#include <iostream>

Resource_manager &Resource_manager::instance()
{
    static Resource_manager manager;
    return manager;
}

// Models

std::shared_ptr<Model> Resource_manager::load_model(const std::string &path)
{
    auto it = models.find(path);
    if (it != models.end())
        return it->second;

    auto model = std::make_shared<Model>();
    if (!model->load_from_file(path))
    {
        std::cerr << "❌ Could not load model: " << path << std::endl;
        return nullptr;
    }

    models[path] = model;
    return model;
}

std::shared_ptr<Model> Resource_manager::get_model(const std::string &path)
{
    auto it = models.find(path);
    return (it != models.end()) ? it->second : nullptr;
}

// Shaders

std::shared_ptr<Shader_program> Resource_manager::load_shader(const std::string &vert, const std::string &frag)
{
    std::string key = vert + "|" + frag;
    auto it = shaders.find(key);
    if (it != shaders.end())
    {
        return it->second;
    }

    auto shader = std::make_shared<Shader_program>();
    if (!shader->load_from_files(vert, frag))
    {
        std::cerr << "❌ Could not load shader: " << key << std::endl;
        return nullptr;
    }

    shaders[key] = shader;
    return shader;
}

std::shared_ptr<Shader_program> Resource_manager::get_shader(const std::string &vert, const std::string &frag)
{
    std::string key = vert + "|" + frag;
    auto it = shaders.find(key);
    if (it != shaders.end())
    {
        return it->second;
    }
    return nullptr;
}

// Textures

std::shared_ptr<Texture> Resource_manager::load_texture(const std::string &path)
{
    auto it = textures.find(path);
    if (it != textures.end())
        return it->second;

    auto tex = std::make_shared<Texture>();
    if (!tex->load_from_file(path))
    {
        std::cerr << "❌ Failed to load texture: " << path << std::endl;
        return nullptr;
    }

    textures[path] = tex;
    return tex;
}

std::shared_ptr<Texture> Resource_manager::load_cubemap(const std::vector<std::string> &paths)
{
    // Create a unique key by concatenating paths
    std::string key = "cubemap:";
    for (const auto &p : paths)
        key += p + ";";

    auto it = textures.find(key);
    if (it != textures.end())
    {
        it->second->bind_cubemap();
        return it->second;
    }

    auto tex = std::make_shared<Texture>();
    if (!tex->load_from_files(paths))
    {
        std::cerr << "❌ Failed to load cubemap textures." << std::endl;
        return nullptr;
    }

    textures[key] = tex;
    return tex;
}

void Resource_manager::clear()
{
    models.clear();
    shaders.clear();
    textures.clear();
    std::cout << "🧹 Resource manager cleared.\n";
}