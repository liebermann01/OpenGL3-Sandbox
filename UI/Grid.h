#pragma once

#include "../Models/Object.h"

class Grid : public Object {
public:
    Grid(std::shared_ptr<GPUProgram> shader_program, float size = 10.0f, int num_lines = 20);

    const glm::vec3 getColor() const override;
    void generateGrid(float size, int num_lines);
    void updateGPU() override;
    void draw(const Camera& cam) override;
    const char* getName() const override;
    const const std::vector<glm::vec3>&  getNormals() const override;
    static const char* gridVertexShaderSrc;
    static const char* gridFragmentShaderSrc;
};
