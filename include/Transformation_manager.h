#pragma once

#include "GL_common.h"
#include <vector>
#include <memory>
#include <utility>

class Transform;

enum class TransformParentType
{
    Everything = 0,
    Camera = 1,
    Entity = 2,
    Skybox = 3
};

class Transform_manager : public std::enable_shared_from_this<Transform_manager>
{
private:
    bool calculated;
    glm::mat4 final_matrix;

    // Static transforms
    std::vector<glm::mat4> static_matrices;

    // Dynamic transforms
    std::vector<std::pair<Transform *, glm::mat4>> local_transforms;
    std::vector<std::pair<Transform *, glm::mat4>> global_transforms;

    std::vector<std::weak_ptr<Transform_manager>> parents;
    TransformParentType parent_type = TransformParentType::Everything;

public:
    Transform_manager();
    ~Transform_manager();

    std::shared_ptr<Transform_manager> clone() const;

    void add_parent(std::shared_ptr<Transform_manager> parent_manager);
    bool has_parent() const;
    TransformParentType get_parent_type() const;
    void set_parent_type(TransformParentType type);

    void calculate_transformations();

    // For overriding logic
    void override_final_matrix(const glm::mat4 &matrix);

    // Main getter
    glm::mat4 get_final_matrix();

    // Transform operations
    void add_static_transform(Transform *t);
    void add_local_transform(Transform *t);
    void add_global_transform(Transform *t);

    // Getters
    glm::mat4 get_local_dynamic();
    glm::mat4 get_global_dynamic();
    bool get_calculated() const;

    // Cleanup
    void clear_dynamic_transforms();
    void clear_all_transforms();
    void clear_static_transfers();
};