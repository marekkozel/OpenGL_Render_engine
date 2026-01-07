#include "Transformation_manager.h"
#include "Transformation.h"

#include "Rotation.h"
#include "Scale.h"
#include "Transfer.h"

#include <iostream>

Transform_manager::Transform_manager()
    : calculated(false), final_matrix(1.0f), static_matrices(4, glm::mat4(1.0f))
{
}

Transform_manager::~Transform_manager()
{

    for (auto &t : local_transforms)
        delete t.first;
    for (auto &t : global_transforms)
        delete t.first;
}

std::shared_ptr<Transform_manager> Transform_manager::clone() const
{
    auto new_manager = std::make_shared<Transform_manager>();

    new_manager->static_matrices = this->static_matrices;

    // Deep copy local transforms
    for (const auto &t : this->local_transforms)
    {
        if (t.first)
            new_manager->local_transforms.push_back({t.first->clone(), glm::mat4(1.0f)});
    }

    // Deep copy global transforms
    for (const auto &t : this->global_transforms)
    {
        if (t.first)
            new_manager->global_transforms.push_back({t.first->clone(), glm::mat4(1.0f)});
    }

    // Copy parents
    for (const auto &parent : this->parents)
    {
        if (auto p = parent.lock())
        {
            new_manager->add_parent(p);
        }
    }

    new_manager->parent_type = this->parent_type;

    return new_manager;
}

void Transform_manager::add_parent(std::shared_ptr<Transform_manager> parent_manager)
{
    parents.push_back(parent_manager);
}

void Transform_manager::calculate_transformations()
{
    for (auto &t : local_transforms)
    {

        t.second *= t.first->get_model_matrix();
    }

    for (auto &t : global_transforms)
    {
        t.second *= t.first->get_model_matrix();
    }

    calculated = false;
}

void Transform_manager::override_final_matrix(const glm::mat4 &matrix)
{
    final_matrix = matrix;
    calculated = true;
}

glm::mat4 Transform_manager::get_final_matrix()
{
    if (!calculated)
    {
        glm::mat4 parent_global = glm::mat4(1.0f);

        for (auto &parent : parents)
        {
            if (auto p = parent.lock())
            {
                if (parent_type == TransformParentType::Skybox)
                {
                    glm::mat4 matrix = glm::mat4(1.0f);

                    matrix[3] = p->get_final_matrix()[3];

                    parent_global *= matrix;
                }
                else if (parent_type == TransformParentType::Everything)
                {
                    parent_global *= p->get_final_matrix();
                }
            }
        }

        final_matrix = parent_global * get_global_dynamic() * static_matrices[0] * get_local_dynamic() * static_matrices[1] * static_matrices[2] * static_matrices[3];

        calculated = true;
    }
    return final_matrix;
}

void Transform_manager::add_static_transform(Transform *t)
{
    if (dynamic_cast<Transfer *>(t))
    {
        static_matrices[0] *= t->get_model_matrix();
    }
    else if (dynamic_cast<Rotation *>(t))
    {
        static_matrices[1] *= t->get_model_matrix();
    }
    else if (dynamic_cast<Scale *>(t))
    {
        static_matrices[2] *= t->get_model_matrix();
    }
    else
    {
        static_matrices[3] *= t->get_model_matrix();
    }

    calculated = false;

    delete t;
}

void Transform_manager::add_local_transform(Transform *t)
{
    local_transforms.push_back({t, glm::mat4(1.0f)});
}

void Transform_manager::add_global_transform(Transform *t)
{
    global_transforms.push_back({t, glm::mat4(1.0f)});
}

glm::mat4 Transform_manager::get_local_dynamic()
{
    glm::mat4 local_dynamic = glm::mat4(1.0f);
    for (const auto &t : local_transforms)
    {

        local_dynamic *= t.second;
    }
    return local_dynamic;
}

glm::mat4 Transform_manager::get_global_dynamic()
{
    glm::mat4 global_dynamic = glm::mat4(1.0f);
    for (const auto &t : global_transforms)
    {
        global_dynamic *= t.second;
    }
    return global_dynamic;
}

bool Transform_manager::get_calculated() const
{
    if (calculated && !this->has_parent())
    {
        return true;
    }
    else
        return false;
}

bool Transform_manager::has_parent() const
{
    return !parents.empty();
}

TransformParentType Transform_manager::get_parent_type() const
{
    return parent_type;
}

void Transform_manager::set_parent_type(TransformParentType type)
{
    parent_type = type;
}

void Transform_manager::clear_dynamic_transforms()
{
    for (auto &t : local_transforms)
        delete t.first;
    for (auto &t : global_transforms)
        delete t.first;

    local_transforms.clear();
    global_transforms.clear();

    calculated = false;
}

void Transform_manager::clear_all_transforms()
{
    for (auto &t : local_transforms)
        delete t.first;
    for (auto &t : global_transforms)
        delete t.first;

    local_transforms.clear();
    global_transforms.clear();

    static_matrices = std::vector<glm::mat4>(4, glm::mat4(1.0f));
    calculated = false;
}

void Transform_manager::clear_static_transfers()
{
    static_matrices[0] = glm::mat4(1.0f);
    calculated = false;
}