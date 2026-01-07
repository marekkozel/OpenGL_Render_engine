#pragma once

#include "GL_common.h"
#include <vector>
#include <memory>
#include <string>

class Entity;
class Camera;
class Spawner;
class Controls;
class Drawable_object;

class Scene
{
private:
    std::vector<std::shared_ptr<Entity>> entities;
    std::shared_ptr<Entity> selected_entity;
    std::shared_ptr<Spawner> spawner;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Entity> skybox;
    int scene_id = -1;

public:
    Scene(int id);
    ~Scene();

    void process(GLFWwindow *window, float delta_time, Controls *controls);
    void add_entity(std::shared_ptr<Entity> entity);

    // Setters
    void set_skybox(std::shared_ptr<Entity> skybox);

    // Getters
    int get_scene_id() const;
    std::shared_ptr<Entity> get_skybox() const;

    // Spawning
    void set_spawn_entity(std::shared_ptr<Entity> entity);
    std::shared_ptr<Entity> get_spawn_entity() const;
    void set_random_spawn(float timer, glm::vec3 min_position, glm::vec3 max_position);

    void spawn_entity(Controls *controls);
    void spawn_random(float delta_time);
    void select_entity(Controls *controls);
    void move_entity(Controls *controls, float delta_time);
    void remove_entity(Controls *controls);

    void bezier_curve_update(Controls *controls);
    void spawn_from_points(Controls *controls);
    void select_spawn_entity(Controls *controls);
};

// Scene Factory Functions
Scene *login_scene();
Scene *whack_the_mole();
Scene *sphere_scene();
Scene *forest_scene();
Scene *solar_system_scene();