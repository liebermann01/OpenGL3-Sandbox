#include <cmath>
#include <memory>
#include "glm/glm.hpp"
#include "Cylinder.h"
#include "imgui/imgui.h"
#include "../../GPUProgram.h"

Cylinder::Cylinder(std::shared_ptr<GPUProgram> shader_program, int segments, float height, float radius)
    : Object(GL_TRIANGLES, shader_program), segments(segments), height(height), radius(radius)
{
    std::vector<glm::vec3> verts;

    float halfHeight = height / 2.0f;

    for (int i = 0; i < segments; ++i) {
        float angle0 = (float)i / segments * glm::two_pi<float>();
        float angle1 = (float)(i + 1) / segments * glm::two_pi<float>();

        glm::vec3 p0(radius * cos(angle0), -halfHeight, radius * sin(angle0));
        glm::vec3 p1(radius * cos(angle1), -halfHeight, radius * sin(angle1));
        glm::vec3 p2(radius * cos(angle0), +halfHeight, radius * sin(angle0));
        glm::vec3 p3(radius * cos(angle1), +halfHeight, radius * sin(angle1));

        verts.push_back(p0);
        verts.push_back(p2);
        verts.push_back(p1);

        verts.push_back(p1);
        verts.push_back(p2);
        verts.push_back(p3);

        verts.push_back(glm::vec3(0.0f, -halfHeight, 0.0f));
        verts.push_back(p1);
        verts.push_back(p0);

        verts.push_back(glm::vec3(0.0f, +halfHeight, 0.0f));
        verts.push_back(p2);
        verts.push_back(p3);
    }

    vertices->Vtx() = verts;

    calculateNormals();
    updateGPU();
}

void Cylinder::calculateNormals() {
    normals.clear();
    normals.reserve(vertices->Vtx().size());

    for (const auto& v : vertices->Vtx()) {
        glm::vec3 n = glm::normalize(glm::vec3(v.x, 0.0f, v.z)); 
        normals.push_back(n);
    }
}

const std::vector<glm::vec3>& Cylinder::getNormals() const {
    return normals;
}

void Cylinder::updateGPU() {
    vertices->updateGPU();
}

const glm::vec3 Cylinder::getColor() const {
    return color;
}

const char* Cylinder::getName() const {
    return "Cylinder";
}

void Cylinder::renderUI() {
    ImGui::PushID(id);
    Object::renderUI();
    //ImGui::Text("Cylinder-specific properties");
    if (ImGui::ColorEdit3("Color", &color.x)) {
        updateGPU();
    }
    ImGui::PopID();
}
