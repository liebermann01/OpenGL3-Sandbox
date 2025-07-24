#include "Object.h"

int Object::nextId = 0;

const char* Object::vertexShaderSrc = R"(
    #version 330 core

    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    out vec3 fragNormal;
    out vec3 fragPosition;

    void main() {
        fragPosition = vec3(model * vec4(aPos, 1.0));
        fragNormal = mat3(transpose(inverse(model))) * aNormal;
        gl_Position = projection * view * vec4(fragPosition, 1.0);
    }

)";

const char* Object::fragmentShaderSrc = R"(
        #version 330 core

        in vec3 fragNormal;
        in vec3 fragPosition;

        uniform vec3 color;           // objektum alapszíne
        uniform vec3 lightDirection;  // irányított fény iránya (világtérben)
        uniform vec3 lightColor;      // fény színe
        uniform float ambientStrength; // környezeti fény erõssége

        out vec4 FragColor;

        void main() {
            // Normalizált normál és irány
            vec3 norm = normalize(fragNormal);
            vec3 lightDir = normalize(-lightDirection); // fontos: ellenkezõ irány

            // Diffúz fény
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;

            // Ambient komponens
            vec3 ambient = ambientStrength * lightColor;

            // Összesített szín
            vec3 result = (ambient + diffuse) * color;
            FragColor = vec4(result, 1.0);
        }
)";



int Object::getId() const { 
    return id;
}

void Object::setId(int newId) {
    id = newId;
}

Object::Object(GLenum primitiveType, std::shared_ptr<GPUProgram> shaderProgram)
    : primitiveType(primitiveType), program(std::move(shaderProgram))
{
    vertices = std::make_unique<Geometry<glm::vec3>>();
    updateModelMatrix();

    id = nextId++;
}

bool Object::isMarkedForRemoval() const {
    return shouldBeRemoved;
}


void Object::renderUI() {

    ImGui::Checkbox("Active", &active);

    ImGui::SameLine();

    if (ImGui::Button("Remove")) {
        shouldBeRemoved = true;
    }

    ImGui::Separator();


    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        glm::vec3 scale = transform.scale;
        ImGui::Text("Scale:");
        ImGui::PushItemWidth(60);
        ImGui::InputFloat("##ScaleX", &scale.x); ImGui::SameLine();
        ImGui::InputFloat("##ScaleY", &scale.y); ImGui::SameLine();
        ImGui::InputFloat("##ScaleZ", &scale.z);
        ImGui::PopItemWidth();
        if (ImGui::IsItemDeactivatedAfterEdit() || true) {
            transform.scale = scale;
            updateGPU();
        }

        glm::vec3 rot = transform.rotation;
        ImGui::Text("Rotation:");
        ImGui::PushItemWidth(60);
        ImGui::InputFloat("##RotX", &rot.x); ImGui::SameLine();
        ImGui::InputFloat("##RotY", &rot.y); ImGui::SameLine();
        ImGui::InputFloat("##RotZ", &rot.z);
        ImGui::PopItemWidth();
        if (ImGui::IsItemDeactivatedAfterEdit() || true) {
            transform.rotation = rot;
            updateGPU();
        }

        glm::vec3 trans = transform.position;
        ImGui::Text("Position:");
        ImGui::PushItemWidth(60);
        ImGui::InputFloat("##TransX", &trans.x); ImGui::SameLine();
        ImGui::InputFloat("##TransY", &trans.y); ImGui::SameLine();
        ImGui::InputFloat("##TransZ", &trans.z);
        ImGui::PopItemWidth();
        if (ImGui::IsItemDeactivatedAfterEdit() || true) {
            transform.position = trans;
            updateGPU();
        }
    }

    ImGui::Separator();
}

Geometry<glm::vec3>* Object::getGeometry() {
    return vertices.get();
}

std::shared_ptr<GPUProgram> Object::getProgram() const { 
    return program;
}

const glm::mat4& Object::getModelMatrix() const {
    return modelMatrix;
}

const glm::vec3& Object::getTranslation() const {
   return translation;
}

void Object::setTranslation(const glm::vec3& t) {
    translation = t;
    updateModelMatrix();
}

void Object::updateModelMatrix() {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(transform.getModelMatrix(), translation);
    modelMatrix = glm::rotate(transform.getModelMatrix(), glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));
    modelMatrix = glm::rotate(transform.getModelMatrix(), glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(transform.getModelMatrix(), glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));
    modelMatrix = glm::scale(transform.getModelMatrix(), transform.scale);
}

const char* Object::getName() const {
    return "Unnamed Object";
}

void  Object::update(float dt) {
    updateModelMatrix();
}