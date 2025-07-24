#include "EditorScene.h"
#include "../Models/Object.h"
#include "../Models/Shapes/Sphere.h"
#include "../Models/Shapes/Pyramid.h"
#include "../Models/Shapes/Cylinder.h"
#include "../UI/Grid.h"

void EditorScene::setup() {
    auto program = std::make_shared<GPUProgram>(Grid::gridVertexShaderSrc, Grid::gridFragmentShaderSrc);

    std::shared_ptr<Grid> grid = std::make_shared<Grid>(program, 2000.0f, 2000);
    this->addObject(grid);
}

void EditorScene::update(float dt) {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(250, ImGui::GetIO().DisplaySize.y), ImGuiCond_Always);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_MenuBar;



    ImGui::Begin("Scene", nullptr, window_flags);

    if (ImGui::BeginMenuBar()) {
        ImGui::Checkbox("Show Grid", &grid_layer);
        ImGui::EndMenuBar();
    }

    static const char* items[] = { "Cube", "Sphere", "Pyramid", "Cylinder"};
    static int current_item = -1;

    ImGui::PushItemWidth(150);
    ImGui::Combo("##combo", &current_item, items, IM_ARRAYSIZE(items));
    ImGui::PopItemWidth();

    ImGui::SameLine();

    bool disabled = current_item == -1;
    ImGui::BeginDisabled(disabled);

    if (ImGui::Button("Add", ImVec2(60, 0))) {
        auto program = std::make_shared<GPUProgram>(Grid::gridVertexShaderSrc, Grid::gridFragmentShaderSrc);
        if (current_item != -1) {
            std::string selected_type = items[current_item];

            if (selected_type == "Cube") {
                std::shared_ptr<Cube> cube = std::make_shared<Cube>(program);
                this->addObject(cube);
            }
            if (selected_type == "Sphere") {
                std::shared_ptr<Sphere> cube = std::make_shared<Sphere>(program);
                this->addObject(cube);

            }
            if (selected_type == "Pyramid") {
                std::shared_ptr<Pyramid> pyramid = std::make_shared<Pyramid>(program);
                this->addObject(pyramid);
            }
            else if (selected_type == "Cylinder") {
                std::shared_ptr<Cylinder> pyramid = std::make_shared<Cylinder>(program);
                this->addObject(pyramid);
            }

            current_item = -1; 
        }
    }

    ImGui::Separator();

    ImGui::EndDisabled();

    Scene::update(dt);

}