#pragma once

#define GLM_ENABLE_EXPERIMENTAL 

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <GLFW/glfw3.h>
#include <cmath>

class Camera
{
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    float movementSpeed;
    float mouseSensitivity;

    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

    float lastX = 960.0f, lastY = 540.0f;
    bool firstMouse = true;

    Camera(
        glm::vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f),
        float Yaw = -90.0f,
        float Pitch = 0.0f,
        float Fov = 45.0f,
        float aspectRatio = 4.0f / 3.0f,
        float nearPlane = 0.1f,
        float farPlane = 100.0f
    );

    void processInput(GLFWwindow* window, float deltaTime);

    void updateCursorMode(GLFWwindow* window, bool shiftPressed);

    glm::mat4 getViewMatrix() const;

    glm::mat4 getProjectionMatrix() const;

    void processKeyboard(char direction, float delta_time);

    void processMouseMovement(float xoffset, float yoffset, bool constrain_pitch = true);

    void handleMouseCallback(GLFWwindow* window, double xpos, double ypos);

    void Update(float dt, GLFWwindow* window);


private:
    void updateCameraVectors();
};
