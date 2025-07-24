#include "Sphere.h"
#include <glm/glm.hpp>
#include <vector>
#include <cmath>
#include "imgui/imgui.h"

Sphere::Sphere(std::shared_ptr<GPUProgram> shader_program, int segments, int rings, float radius)
    : Object(GL_TRIANGLES, shader_program)
{
    layer = 1;
    active = true;

    std::vector<glm::vec3> verts;

    for (int y = 0; y <= rings; ++y) {
        float v = float(y) / rings;
        float theta = v * glm::pi<float>();

        for (int x = 0; x <= segments; ++x) {
            float u = float(x) / segments;
            float phi = u * glm::two_pi<float>();

            float sinTheta = sin(theta);
            glm::vec3 pos(
                radius * cos(phi) * sinTheta,
                radius * cos(theta),
                radius * sin(phi) * sinTheta
            );
            verts.push_back(pos);
        }
    }

    std::vector<glm::vec3> triangles;
    for (int y = 0; y < rings; ++y) {
        for (int x = 0; x < segments; ++x) {
            int i0 = y * (segments + 1) + x;
            int i1 = i0 + 1;
            int i2 = i0 + segments + 1;
            int i3 = i2 + 1;

            // két háromszög
            triangles.push_back(verts[i0]);
            triangles.push_back(verts[i2]);
            triangles.push_back(verts[i1]);

            triangles.push_back(verts[i1]);
            triangles.push_back(verts[i2]);
            triangles.push_back(verts[i3]);
        }
    }

    vertices->Vtx() = triangles;

    calculateNormals();

    updateGPU();
}

void Sphere::calculateNormals() {
    normals.clear();
    normals.reserve(vertices->Vtx().size());

    for (const auto& v : vertices->Vtx()) {
        glm::vec3 n = glm::normalize(v); 
        normals.push_back(n);
    }
}

const std::vector<glm::vec3>& Sphere::getNormals() const {
    return normals;
}

void Sphere::updateGPU() {
    vertices->updateGPU();
}

void Sphere::draw(const Camera& cam) {
    program->Use();
    program->setUniform(getModelMatrix(), "model");
    program->setUniform(cam.getViewMatrix(), "view");
    program->setUniform(cam.getProjectionMatrix(), "projection");
    program->setUniform(color, "color");
    vertices->Bind();
    glDrawArrays(primitiveType, 0, static_cast<GLsizei>(vertices->Vtx().size()));
}

const char* Sphere::getName() const {
    return "Sphere";
}

const glm::vec3 Sphere::getColor() const {
    return color;
}

void Sphere::renderUI() {
    ImGui::PushID(id);
    Object::renderUI();
    //ImGui::Text("Sphere-specific properties");
    if (ImGui::ColorEdit3("Color", &color.x)) {
        updateGPU();
    }
    ImGui::PopID();
}