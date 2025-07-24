#include "Cube.h"

Cube::Cube(std::shared_ptr<GPUProgram> prog)
    : Object(GL_TRIANGLES, prog)
{
    layer = 1;
    active = true;

    vertices->Vtx() = {
        {-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f},
        {-0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}
    };

    std::vector<unsigned int> indices = {
        0,1,2, 2,3,0,
        4,5,6, 6,7,4,
        4,0,3, 3,7,4,
        1,5,6, 6,2,1,
        4,5,1, 1,0,4,
        3,2,6, 6,7,3
    };

    std::vector<glm::vec3> expandedVertices;
    for (auto idx : indices) {
        expandedVertices.push_back(vertices->Vtx()[idx]);
    }
    vertices->Vtx() = expandedVertices;

    calculateNormals();

    updateGPU();
}

void Cube::calculateNormals() {
    const auto& verts = vertices->Vtx();

    normals.clear();
    normals.resize(verts.size(), glm::vec3(0.0f));

    for (size_t i = 0; i < verts.size(); i += 3) {
        glm::vec3 v0 = verts[i];
        glm::vec3 v1 = verts[i + 1];
        glm::vec3 v2 = verts[i + 2];

        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;

        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

        normals[i] += normal;
        normals[i + 1] += normal;
        normals[i + 2] += normal;
    }

    for (auto& n : normals) {
        n = glm::normalize(n);
    }
}

const std::vector<glm::vec3>& Cube::getNormals() const {
    return normals;
}


void Cube::updateGPU() {
    vertices->updateGPU();
}

const char* Cube::getName() const {
    return "Cube";
}

const glm::vec3 Cube::getColor() const {
    return color;
}

void Cube::renderUI()  {
    ImGui::PushID(id);
    Object::renderUI();
    //ImGui::Text("Cube-specific properties");
    if (ImGui::ColorEdit3("Color", &color.x)) {
        updateGPU();
    }
    ImGui::PopID();
}

void Cube::draw(const Camera& cam) {
    program->Use();
    program->setUniform(this->getModelMatrix(), "model");
    program->setUniform(cam.getViewMatrix(), "view");
    program->setUniform(cam.getProjectionMatrix(), "projection");
    program->setUniform(color, "color");
    vertices->Bind();
    glDrawArrays(primitiveType, 0, (GLsizei)vertices->Vtx().size());
}