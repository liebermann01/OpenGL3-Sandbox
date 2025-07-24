#include "Grid.h"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

const char* Grid::gridVertexShaderSrc = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
)";

const char* Grid::gridFragmentShaderSrc = R"(
    #version 330 core

    uniform vec3 color;

    out vec4 FragColor;

    void main() {
        FragColor = vec4(color, 1.0);
    }
)";



Grid::Grid(std::shared_ptr<GPUProgram> shader_program, float size, int num_lines)
    : Object(GL_LINES, shader_program)
{
    layer = 0;
    active = true;
    generateGrid(size, num_lines);
    updateGPU();
}

const glm::vec3 Grid::getColor() const {
    return glm::vec3(0.5f, 0.5f, 0.5f);
}

void Grid::generateGrid(float size, int num_lines) {
    float half = size / 2.0f;
    float step = size / static_cast<float>(num_lines);
    auto& verts = vertices->Vtx();
    verts.clear();

    for (int i = 0; i <= num_lines; ++i) {
        float pos = -half + i * step;
        verts.emplace_back(glm::vec3(-half, 0.0f, pos));
        verts.emplace_back(glm::vec3(half, 0.0f, pos));
        verts.emplace_back(glm::vec3(pos, 0.0f, -half));
        verts.emplace_back(glm::vec3(pos, 0.0f, half));
    }
}

void Grid::updateGPU() {
    vertices->updateGPU();
}

void Grid::draw(const Camera& cam) {
    program->Use();
    program->setUniform(this->getModelMatrix(), "model");
    program->setUniform(cam.getViewMatrix(), "view");
    program->setUniform(cam.getProjectionMatrix(), "projection");

    program->setUniform(getColor(), "color");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glLineWidth(1.0f);
    vertices->Bind();
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertices->Vtx().size()));
    glDisable(GL_BLEND);
}

const char* Grid::getName() const {
    return "Grid";
}

const std::vector<glm::vec3>& Grid::getNormals() const {
    static const std::vector<glm::vec3> emptyNormals;
    return emptyNormals;
}