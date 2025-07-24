#define GLM_ENABLE_EXPERIMENTAL 

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <memory>
#include "GPUProgram.h"
#include "Cameras/Camera.h"
#include "UI/UIManager.h"
#include "Scenes/EditorScene.h"

Camera cam;
float lastX = 960.0f, lastY = 540.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) && !glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT))
        return;

    if (ImGui::GetIO().WantCaptureMouse)
        return;

    if (firstMouse)
    {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos;

    lastX = (float)xpos;
    lastY = (float)ypos;

    cam.processMouseMovement(xoffset, yoffset);
}

int main(int argc, char** argv)
{
    if (!glfwInit())
        return -1;

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "OpenGL Sandbox", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    glEnable(GL_DEPTH_TEST);

    glfwSetCursorPosCallback(window, mouse_callback);

    cam = Camera(
        glm::vec3(0, 5, 5),
        glm::vec3(0, 1, 0),
        -90.0f, 0.0f,
        45.0f,
        1920.0f / 1080.0f,
        0.1f, 100.0f
    );

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    EditorScene scene;
    scene.setup();

    UIManager um(scene.getObjects());

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        cam.Update(deltaTime, window);

        scene.update(deltaTime);
        scene.draw(cam);

        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}