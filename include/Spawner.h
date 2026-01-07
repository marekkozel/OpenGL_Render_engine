#pragma once

#include "GL_common.h"
#include <vector>
#include <memory>

class Entity;

class Spawner
{
private:
    std::shared_ptr<Entity> entity_to_spawn;

    float spawn_timer = 0.0f;
    float current_timer = 0.0f;

    glm::vec3 min_position = glm::vec3(0.0f);
    glm::vec3 max_position = glm::vec3(0.0f);

    std::vector<glm::vec3> point_array;

    float random_float(float min, float max);

public:
    Spawner();
    ~Spawner();

    // Spawning
    std::shared_ptr<Entity> spawn_entity(glm::vec3 position) const;
    std::shared_ptr<Entity> random_spawn(float delta_time);
    std::shared_ptr<Entity> spawn_from_points();

    void set_entity_to_spawn(std::shared_ptr<Entity> entity);
    std::shared_ptr<Entity> get_entity_to_spawn();

    void set_random_spawn(float timer, glm::vec3 min_position, glm::vec3 max_position);

    void add_spawn_point(glm::vec3 point);
    void delete_spawn_points();
};