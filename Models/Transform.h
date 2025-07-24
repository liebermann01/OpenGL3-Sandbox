#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
public:
    glm::vec3 position  { 0.0f };
    glm::vec3 rotation  { 0.0f }; 
    glm::vec3 scale     { 1.0f };

    Transform() = default;

    glm::mat4 getModelMatrix() const;
};

