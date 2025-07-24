#pragma once
#include "glm/glm.hpp"
#include "../Object.h"


class Cylinder : public Object {
    glm::vec3 color = glm::vec3(0.8f, 0.2f, 0.1f);
    int segments;
    float height;
    float radius;

    void calculateNormals();

public:
    Cylinder(std::shared_ptr<GPUProgram> shader_program, int segments = 32, float height = 2.0f, float radius = 1.0f);

    const std::vector<glm::vec3>& getNormals() const override;
    const glm::vec3 getColor() const override;
    void updateGPU() override;
    const char* getName() const override;
    void renderUI() override;
};
