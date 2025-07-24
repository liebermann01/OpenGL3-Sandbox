#include "Transform.h"

glm::mat4 Transform::getModelMatrix() const {
    glm::mat4 mat = glm::mat4(1.0f);

    mat = glm::translate(mat, position);

    mat = glm::rotate(mat, rotation.x, glm::vec3(1, 0, 0));
    mat = glm::rotate(mat, rotation.y, glm::vec3(0, 1, 0));
    mat = glm::rotate(mat, rotation.z, glm::vec3(0, 0, 1));

    mat = glm::scale(mat, scale);

    return mat;
}