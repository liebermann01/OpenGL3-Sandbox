#include "GPUProgram.h"
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>
#include <cstdlib>

GPUProgram::GPUProgram() {}

GPUProgram::GPUProgram(const char* const vertexShaderSource, const char* const fragmentShaderSource, const char* const geometryShaderSource) {
    create(vertexShaderSource, fragmentShaderSource, geometryShaderSource);
}

GPUProgram::~GPUProgram() {
    if (shaderProgramId > 0)
        glDeleteProgram(shaderProgramId);
}

bool GPUProgram::checkShader(unsigned int shader, std::string message) {
    GLint infoLogLength = 0, result = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (!result) {
        std::string errorMessage(infoLogLength, '\0');
        glGetShaderInfoLog(shader, infoLogLength, NULL, (GLchar*)errorMessage.data());
        printf("%s! \nLog:\n%s\n", message.c_str(), errorMessage.c_str());
        if (waitError) getchar();
        return false;
    }
    return true;
}

bool GPUProgram::checkLinking(unsigned int program) {
    GLint infoLogLength = 0, result = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (!result) {
        std::string errorMessage(infoLogLength, '\0');
        glGetProgramInfoLog(program, infoLogLength, nullptr, (GLchar*)errorMessage.data());
        printf("Failed to link shader program! \nLog:\n%s\n", errorMessage.c_str());
        if (waitError) getchar();
        return false;
    }
    return true;
}

int GPUProgram::getLocation(const std::string& name) {
    int location = glGetUniformLocation(shaderProgramId, name.c_str());
    if (location < 0)
        printf("uniform %s cannot be set\n", name.c_str());
    return location;
}

void GPUProgram::create(const char* const vertexShaderSource, const char* const fragmentShaderSource, const char* const geometryShaderSource) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    if (!vertexShader) {
        printf("Error in vertex shader creation\n");
        exit(1);
    }
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    if (!checkShader(vertexShader, "Vertex shader error")) return;

    GLuint geometryShader = 0;
    if (geometryShaderSource != nullptr) {
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        if (!geometryShader) {
            printf("Error in geometry shader creation\n");
            exit(1);
        }
        glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
        glCompileShader(geometryShader);
        if (!checkShader(geometryShader, "Geometry shader error")) return;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    if (!fragmentShader) {
        printf("Error in fragment shader creation\n");
        exit(1);
    }
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    if (!checkShader(fragmentShader, "Fragment shader error")) return;

    shaderProgramId = glCreateProgram();
    if (!shaderProgramId) {
        printf("Error in shader program creation\n");
        exit(-1);
    }

    glAttachShader(shaderProgramId, vertexShader);
    glAttachShader(shaderProgramId, fragmentShader);
    if (geometryShader > 0) glAttachShader(shaderProgramId, geometryShader);

    glBindFragDataLocation(shaderProgramId, 0, "fragmentColor");

    if (!link()) return;

    glUseProgram(shaderProgramId);
}

bool GPUProgram::link() {
    glLinkProgram(shaderProgramId);
    return checkLinking(shaderProgramId);
}

void GPUProgram::Use() {
    glUseProgram(shaderProgramId);
}

void GPUProgram::setUniform(int i, const std::string& name) {
    int location = getLocation(name);
    if (location >= 0) glUniform1i(location, i);
}

void GPUProgram::setUniform(float f, const std::string& name) {
    int location = getLocation(name);
    if (location >= 0) glUniform1f(location, f);
}

void GPUProgram::setUniform(const glm::vec2& v, const std::string& name) {
    int location = getLocation(name);
    if (location >= 0) glUniform2fv(location, 1, &v.x);
}

void GPUProgram::setUniform(const glm::vec3& v, const std::string& name) {
    int location = getLocation(name);
    if (location >= 0) glUniform3fv(location, 1, &v.x);
}

void GPUProgram::setUniform(const glm::vec4& v, const std::string& name) {
    int location = getLocation(name);
    if (location >= 0) glUniform4fv(location, 1, &v.x);
}

void GPUProgram::setUniform(const glm::mat4& mat, const std::string& name) {
    int location = getLocation(name);
    if (location >= 0) glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}
