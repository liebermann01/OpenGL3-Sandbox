#pragma once
#include "../Object.h"

class Cube : public Object {
public:
    glm::vec3 color = { 1.0f, 0.0f, 0.0f };

    Cube(std::shared_ptr<GPUProgram> prog);

    void updateGPU() override;

    const char* getName() const override;

    const glm::vec3 getColor() const override;

    void renderUI() override;

    void draw(const Camera& cam) override;

    const std::vector<glm::vec3>& getNormals() const;

private:
    std::vector<glm::vec3> normals;

    void calculateNormals();
};


