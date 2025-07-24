#pragma once

#include "glm/glm.hpp"
#include <string>
#include <glad/glad.h>

class GPUProgram {
private:
    GLuint shaderProgramId = 0;
    bool waitError = true;

    bool checkShader(unsigned int shader, std::string message);
    bool checkLinking(unsigned int program);
    int getLocation(const std::string& name);

public:
    GPUProgram();
    GPUProgram(const char* const vertexShaderSource, const char* const fragmentShaderSource, const char* const geometryShaderSource = nullptr);
    ~GPUProgram();

    void create(const char* const vertexShaderSource, const char* const fragmentShaderSource, const char* const geometryShaderSource = nullptr);
    bool link();
    void Use();

    void setUniform(int i, const std::string& name);
    void setUniform(float f, const std::string& name);
    void setUniform(const glm::vec2& v, const std::string& name);
    void setUniform(const glm::vec3& v, const std::string& name);
    void setUniform(const glm::vec4& v, const std::string& name);
    void setUniform(const glm::mat4& mat, const std::string& name);
};
