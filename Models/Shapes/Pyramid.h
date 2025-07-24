#pragma once

#include "../Object.h"

class Pyramid : public Object {
public:
    Pyramid(std::shared_ptr<GPUProgram> shader_program);

    virtual const glm::vec3 getColor() const override;
    virtual void updateGPU() override;
    virtual void draw(const Camera& cam) override;
    virtual const char* getName() const override;
    virtual void renderUI() override;

    const std::vector<glm::vec3>& getNormals() const;

private:
    glm::vec3 color = glm::vec3(1.0f, 0.8f, 0.2f);

    std::vector<glm::vec3> normals;

    void calculateNormals();
};
