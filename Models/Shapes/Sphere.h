#pragma once

#include "../Object.h"
#include "glm/glm.hpp"

class Sphere : public Object {
public:
    Sphere(std::shared_ptr<GPUProgram> shader_program, int segments = 16, int rings = 16, float radius = 0.5f);

    virtual const glm::vec3 getColor() const override;
    virtual void updateGPU() override;
    virtual void draw(const Camera& cam) override;
    virtual const char* getName() const override;
    virtual void renderUI() override;

    const std::vector<glm::vec3>& getNormals() const;


private:
    glm::vec3 color = glm::vec3(0.2f, 0.6f, 1.0f);

    std::vector<glm::vec3> normals;

    void calculateNormals();
};
