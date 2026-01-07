#include "Spawner.h"
#include "Entity.h"
#include "Transformation_manager.h"

#include "TR_common.h"

#include <cstdlib>
#include <iostream>

Spawner::Spawner() {}

Spawner::~Spawner() {}

std::shared_ptr<Entity> Spawner::spawn_entity(glm::vec3 position) const
{
    if (entity_to_spawn)
    {
        auto new_entity = entity_to_spawn->clone();

        // Reset position to the click location
        new_entity->get_transform_manager()->clear_static_transfers();
        new_entity->add_static_transform(new Transfer(position));

        return new_entity;
    }
    return nullptr;
}

std::shared_ptr<Entity> Spawner::random_spawn(float delta_time)
{
    if (entity_to_spawn && spawn_timer > 0.0f)
    {
        current_timer += delta_time;

        if (current_timer >= spawn_timer)
        {
            current_timer = 0;
            auto new_entity = entity_to_spawn->clone();

            glm::vec3 random_pos(
                random_float(min_position.x, max_position.x),
                random_float(min_position.y, max_position.y),
                random_float(min_position.z, max_position.z));

            new_entity->add_static_transform(new Transfer(random_pos));

            // std::cout << "Spawned Entity ID: " << new_entity->get_id() << std::endl;
            return new_entity;
        }
    }
    return nullptr;
}

std::shared_ptr<Entity> Spawner::spawn_from_points()
{
    if (entity_to_spawn && !point_array.empty())
    {
        auto new_entity = entity_to_spawn->clone();

        new_entity->get_transform_manager()->clear_static_transfers();
        new_entity->get_transform_manager()->clear_dynamic_transforms();

        // Apply Bezier path
        new_entity->add_local_transform(new Bezier_transform(point_array, 0.5f));

        return new_entity;
    }
    return nullptr;
}

float Spawner::random_float(float min, float max)
{
    return ((float)rand() / RAND_MAX) * (max - min) + min;
}

void Spawner::set_entity_to_spawn(std::shared_ptr<Entity> entity)
{
    entity_to_spawn = entity;
}

std::shared_ptr<Entity> Spawner::get_entity_to_spawn()
{
    return entity_to_spawn;
}

void Spawner::set_random_spawn(float timer, glm::vec3 min_pos, glm::vec3 max_pos)
{
    this->spawn_timer = timer;
    this->min_position = min_pos;
    this->max_position = max_pos;
}

void Spawner::add_spawn_point(glm::vec3 point)
{
    point_array.push_back(point);
}

void Spawner::delete_spawn_points()
{
    point_array.clear();
}