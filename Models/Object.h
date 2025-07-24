#pragma once
#include "../Geometry.h"
#include "../GPUProgram.h"
#include "../Lights/Light.h"
#include "../Cameras/Camera.h"
#include <memory>
#include <glm/glm.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Transform.h"

class Object {
protected:
    std::unique_ptr<Geometry<glm::vec3>> vertices;
    std::shared_ptr<GPUProgram> program;

    Transform transform;
    glm::vec3 translation = glm::vec3(0.0f);
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    static int nextId;
    int id;

    std::vector<glm::vec3> normals;
    GLuint normalVBO = 0;
   
   
public:
    bool shouldBeRemoved = false;
    static const char* vertexShaderSrc;
    static const char* fragmentShaderSrc;
    bool active = true;

    bool isMarkedForRemoval() const;

     unsigned int layer = 1;
     int getId() const;
     void setId(int newId);

    const GLenum primitiveType;

    Object(GLenum primitiveType, std::shared_ptr<GPUProgram> shaderProgram);

    virtual ~Object() = default;

    virtual const glm::vec3 getColor() const = 0;

    virtual void updateGPU() = 0;

    virtual void draw(const Camera& cam) {

        program->Use();

        program->setUniform(modelMatrix, "model");
        program->setUniform(cam.getViewMatrix(), "view");
        program->setUniform(cam.getProjectionMatrix(), "projection");
        program->setUniform(getColor(), "color");

        glm::vec3 lightDir = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)); 
        glm::vec3 lightColor = glm::vec3(1.0f); 
        float ambientStrength = 0.9f;

        program->setUniform(lightDir, "lightDirection");
        program->setUniform(lightColor, "lightColor");
        program->setUniform(ambientStrength, "ambientStrength");

        vertices->Bind();
        glDrawArrays(primitiveType, 0, (GLsizei)vertices->Vtx().size());
    }

    virtual void renderUI();

    Geometry<glm::vec3>* getGeometry();
    std::shared_ptr<GPUProgram> getProgram() const;

    const glm::mat4& getModelMatrix() const;

    const glm::vec3& getTranslation() const;
    void setTranslation(const glm::vec3& translation);

    virtual void updateModelMatrix();

    virtual const char* getName() const;
    virtual void update(float dt);

    virtual const std::vector<glm::vec3>& getNormals() const = 0;

};