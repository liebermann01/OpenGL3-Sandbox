#include "Pyramid.h"
#include "imgui/imgui.h"

Pyramid::Pyramid(std::shared_ptr<GPUProgram> shader_program)
    : Object(GL_TRIANGLES, shader_program)
{
    layer = 1;
    active = true;

    // Alap négy pontja (négyzet)
    glm::vec3 a(-0.5f, 0.0f, -0.5f);
    glm::vec3 b(0.5f, 0.0f, -0.5f);
    glm::vec3 c(0.5f, 0.0f, 0.5f);
    glm::vec3 d(-0.5f, 0.0f, 0.5f);

    // Csúcs
    glm::vec3 top(0.0f, 1.0f, 0.0f);

    std::vector<glm::vec3> tris = {
        // Alap (2 háromszög)
        a, b, c,
        c, d, a,

        // Oldallapok (4 háromszög)
        a, b, top,
        b, c, top,
        c, d, top,
        d, a, top
    };

    vertices->Vtx() = tris;

    calculateNormals();

    updateGPU();
}

void Pyramid::calculateNormals() {
    normals.clear();
    normals.reserve(vertices->Vtx().size());

    for (size_t i = 0; i < vertices->Vtx().size(); i += 3) {
        glm::vec3 v0 = vertices->Vtx()[i];
        glm::vec3 v1 = vertices->Vtx()[i + 1];
        glm::vec3 v2 = vertices->Vtx()[i + 2];

        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

        normals.push_back(normal);
        normals.push_back(normal);
        normals.push_back(normal);
    }
}

void Pyramid::updateGPU() {
    vertices->updateGPU();
}

void Pyramid::draw(const Camera& cam) {
    program->Use();
    program->setUniform(getModelMatrix(), "model");
    program->setUniform(cam.getViewMatrix(), "view");
    program->setUniform(cam.getProjectionMatrix(), "projection");
    program->setUniform(color, "color");
    vertices->Bind();
    glDrawArrays(primitiveType, 0, static_cast<GLsizei>(vertices->Vtx().size()));
}

const glm::vec3 Pyramid::getColor() const {
    return color;
}

const char* Pyramid::getName() const {
    return "Pyramid";
}

void Pyramid::renderUI() {
    ImGui::PushID(id);
    Object::renderUI();
    ImGui::Text("Pyramid-specific properties");
    if (ImGui::ColorEdit3("Color", &color.x)) {
        updateGPU();
    }
    ImGui::PopID();
}

const std::vector<glm::vec3>& Pyramid::getNormals() const {
    return normals;
}
