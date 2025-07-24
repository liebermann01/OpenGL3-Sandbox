#pragma once
#include "glm/glm.hpp"
#include <string>
#include <memory>
#include "../GPUProgram.h"

class Light {
public:
    glm::vec3 direction = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));
    glm::vec3 color = glm::vec3(1.0f);
    float ambientStrength = 0.2f;

    Light() = default;

    Light(const glm::vec3& dir, const glm::vec3& col, float ambient)
        : direction(glm::normalize(dir)), color(col), ambientStrength(ambient) {
    }

    void setUniforms(std::shared_ptr<GPUProgram> program) const {
        program->setUniform(direction, "lightDirection");
        program->setUniform(color, "lightColor");
        program->setUniform(ambientStrength, "ambientStrength");
    }
};
