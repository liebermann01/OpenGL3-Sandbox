#pragma once
#include <vector>
#include "../Models/Object.h"
#include <memory>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class UIManager {
    std::vector<std::shared_ptr<Object>>& objects;

public:
    UIManager(std::vector<std::shared_ptr<Object>>& objs);
    void renderUI();
};