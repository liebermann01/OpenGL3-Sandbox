#include "Camera.h"

Camera::Camera(
    glm::vec3 position,
    glm::vec3 up,
    float yaw,
    float pitch,
    float fov,
    float aspectRatio,
    float nearPlane,
    float farPlane
)
    : position(position),
    worldUp(up),
    yaw(yaw),
    pitch(pitch),
    fov(fov),
    aspectRatio(aspectRatio),
    nearPlane(nearPlane),
    farPlane(farPlane),
    movementSpeed(5.0f),
    mouseSensitivity(0.1f)
{
    updateCameraVectors();
}

void Camera::processInput(GLFWwindow* window, float deltaTime)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        processKeyboard('W', deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        processKeyboard('S', deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        processKeyboard('A', deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        processKeyboard('D', deltaTime);
}

void Camera::updateCursorMode(GLFWwindow* window, bool shiftPressed)
{
    static bool cursor_disabled = false;

    if (shiftPressed && !cursor_disabled) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        firstMouse = true;
        cursor_disabled = true;
    }
    else if (!shiftPressed && cursor_disabled) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        cursor_disabled = false;
    }
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::processKeyboard(char Direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;

    if (Direction == 'W')
        position += front * velocity;
    if (Direction == 'S')
        position -= front * velocity;
    if (Direction == 'A')
        position -= right * velocity;
    if (Direction == 'D')
        position += right * velocity;
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if (constrainPitch)
    {
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::handleMouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (!glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) && !glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT))
        return;

    if (ImGui::GetIO().WantCaptureMouse)
        return;

    if (firstMouse) {
        lastX = (float)xPos;
        lastY = (float)yPos;
        firstMouse = false;
    }

    float xOffset = (float)xPos - lastX;
    float yOffset = lastY - (float)yPos;
    lastX = (float)xPos;
    lastY = (float)yPos;

    processMouseMovement(xOffset, yOffset);
}

void Camera::Update(float deltaTime, GLFWwindow* window) {
    bool shiftPressed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;

    if (shiftPressed)
        processInput(window, deltaTime);

    updateCursorMode(window, shiftPressed);
}

void Camera::updateCameraVectors()
{
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}