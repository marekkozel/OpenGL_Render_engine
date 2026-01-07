#include "Complex_transformation.h"

Complex_transform::Complex_transform(std::vector<Transform *> transforms)
    : basic_transforms(transforms) {}

void Complex_transform::add_transformation(Transform *transformation)
{
    basic_transforms.push_back(transformation);
}

glm::mat4 Complex_transform::get_model_matrix()
{
    glm::mat4 model(1.0f);
    for (const auto &transformation : basic_transforms)
    {
        model *= transformation->get_model_matrix();
    }
    return model;
}

Transform *Complex_transform::clone() const
{
    std::vector<Transform *> cloned_transforms;
    for (auto t : basic_transforms)
    {
        cloned_transforms.push_back(t->clone());
    }
    return new Complex_transform(cloned_transforms);
}