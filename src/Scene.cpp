#include "Scene.h"
#include "Entity.h"
#include "Camera.h"
#include "Spawner.h"
#include "Controls.h"
#include "TR_common.h"
#include "Transformation_manager.h"

#include <algorithm>
#include <iostream>

Scene::Scene(int id) : scene_id(id), spawner(std::make_shared<Spawner>())
{
}

Scene::~Scene()
{
    skybox.reset();
    spawner.reset();
    camera.reset();
    selected_entity.reset();

    for (auto &entity : entities)
    {
        entity.reset();
    }
    entities.clear();

    glUseProgram(0);
}

void Scene::add_entity(std::shared_ptr<Entity> entity)
{
    if (entity->get_type() == SubjectType::Camera)
    {
        camera = std::dynamic_pointer_cast<Camera>(entity);
        return;
    }

    entities.push_back(entity);
    entity->notify_observers();
}

void Scene::process(GLFWwindow *window, float delta_time, Controls *controls)
{
    if (skybox)
    {
        skybox->update(window, delta_time, controls);
        skybox->draw();
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    select_spawn_entity(controls);
    bezier_curve_update(controls);
    spawn_from_points(controls);
    spawn_random(delta_time);
    spawn_entity(controls);
    select_entity(controls);
    remove_entity(controls);
    move_entity(controls, delta_time);

    // Update Loop
    for (auto it = entities.begin(); it != entities.end();)
    {
        glStencilFunc(GL_ALWAYS, (*it)->get_id(), 0xFF);

        (*it)->update(window, delta_time, controls);
        (*it)->draw();

        // Time-To-Live logic
        if ((*it)->get_time_to_live() > 0.0f)
        {
            float new_time = (*it)->get_current_time() + delta_time;
            (*it)->set_current_time(new_time);

            if (new_time >= (*it)->get_time_to_live())
            {
                it = entities.erase(it);
                continue;
            }
        }
        ++it;
    }

    if (camera)
    {
        camera->update(window, delta_time, controls);
    }
}

void Scene::select_spawn_entity(Controls *controls)
{
    if (controls->is_key_triggered(GLFW_KEY_L))
    {
        int selected_id = Controls::get_object_id();

        for (auto &entity : entities)
        {
            if (entity->get_id() == selected_id)
            {
                spawner->set_entity_to_spawn(entity);
                std::cout << "Spawner Entity Set: ID " << selected_id << std::endl;
                break;
            }
        }
    }
}

void Scene::bezier_curve_update(Controls *controls)
{
    if (controls->is_key_triggered(GLFW_KEY_B))
    {
        spawner->add_spawn_point(Controls::get_global_mouse_pos());
    }
}

void Scene::spawn_from_points(Controls *controls)
{
    if (controls->is_key_triggered(GLFW_KEY_P))
    {
        auto new_entity = spawner->spawn_from_points();
        if (new_entity)
        {
            entities.push_back(new_entity);
            spawner->delete_spawn_points();
        }
    }
}

void Scene::spawn_entity(Controls *controls)
{
    if (controls->is_key_triggered(GLFW_KEY_E) &&
        get_spawn_entity())
    {
        this->add_entity(spawner->spawn_entity(Controls::get_global_mouse_pos()));
    }
}

void Scene::spawn_random(float delta_time)
{
    auto new_entity = spawner->random_spawn(delta_time);
    if (new_entity)
    {
        entities.push_back(new_entity);
    }
}

void Scene::select_entity(Controls *controls)
{
    if (controls->is_mouse_button_triggered(0))
    {
        int selected_id = Controls::get_object_id();

        for (auto &entity : entities)
        {
            if (entity->get_id() == selected_id)
            {
                selected_entity = entity;
                break;
            }
        }
    }
}

void Scene::remove_entity(Controls *controls)
{
    // Delete currently selected
    if (selected_entity)
    {
        if (controls->is_key_triggered(GLFW_KEY_DELETE))
        {
            entities.erase(std::remove(entities.begin(), entities.end(), selected_entity), entities.end());
            selected_entity.reset();
        }
    }

    // Backspace delete
    if (controls->is_key_triggered(GLFW_KEY_BACKSPACE))
    {
        int selected_id = Controls::get_object_id();
        std::shared_ptr<Entity> target = nullptr;

        for (auto &entity : entities)
        {
            if (entity->get_id() == selected_id)
            {
                target = entity;
                break;
            }
        }

        if (target)
        {
            if (selected_entity == target)
                selected_entity.reset();
            entities.erase(std::remove(entities.begin(), entities.end(), target), entities.end());
        }
    }
}

void Scene::move_entity(Controls *controls, float delta_time)
{
    if (selected_entity && controls->is_mouse_button_pressed(0))
    {
        if (Controls::get_object_id() != selected_entity->get_id())
        {
            glm::mat4 matrix = selected_entity->get_transform_manager()->get_final_matrix();
            glm::vec3 mouse_pos = Controls::get_global_mouse_pos();
            glm::vec3 current_pos = glm::vec3(matrix[3]);

            selected_entity->add_static_transform(new Transfer(mouse_pos - current_pos));
        }
    }
}

// Getters
int Scene::get_scene_id() const { return scene_id; }

std::shared_ptr<Entity> Scene::get_spawn_entity() const
{
    return spawner->get_entity_to_spawn();
}
std::shared_ptr<Entity> Scene::get_skybox() const { return skybox; }

// Setters
void Scene::set_spawn_entity(std::shared_ptr<Entity> entity)
{
    spawner->set_entity_to_spawn(entity);
}
void Scene::set_random_spawn(float timer, glm::vec3 min_position, glm::vec3 max_position)
{
    spawner->set_random_spawn(timer, min_position, max_position);
}
void Scene::set_skybox(std::shared_ptr<Entity> skybox) { this->skybox = skybox; }