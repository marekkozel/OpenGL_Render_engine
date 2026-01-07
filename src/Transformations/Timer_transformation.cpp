#include "Timer_transformation.h"
#include <iostream>

Timer_transform::Timer_transform(std::vector<std::pair<float, Transform *>> transforms)
{
    for (const auto &tt : transforms)
    {
        this->timer_transforms.push_back({tt, 1});
    }
}

void Timer_transform::add_transformation(float timer, Transform *transformation)
{
    timer_transforms.push_back({{timer, transformation}, 1});
}

glm::mat4 Timer_transform::get_model_matrix()
{
    glm::mat4 model(1.0f);

    current_time = (float)glfwGetTime();
    delta_time = current_time - last_time;
    last_time = current_time;

    for (auto &entry : timer_transforms)
    {
        float duration = entry.first.first;
        Transform *transform = entry.first.second;
        float &elapsed = entry.second;

        if (elapsed >= duration)
        {
            if (transform)
            {
                model *= transform->get_model_matrix();
            }
            elapsed = 0;
        }
        elapsed += delta_time;
    }
    return model;
}

Transform *Timer_transform::clone() const
{
    std::vector<std::pair<float, Transform *>> cloned_transforms;
    for (const auto &entry : timer_transforms)
    {
        cloned_transforms.push_back({entry.first.first, entry.first.second->clone()});
    }
    return new Timer_transform(cloned_transforms);
}