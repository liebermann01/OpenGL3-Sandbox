#include "UIManager.h"

UIManager::UIManager(std::vector<std::shared_ptr<Object>>& objs) : objects(objs) {

}

void UIManager::renderUI() {
    ImGui::Begin("Objects Control Panel");

    int index = 0;
    for (auto& obj : objects) {
        ImGui::PushID(index++);
        obj->renderUI();
        ImGui::Separator();
        ImGui::PopID();
    }

    ImGui::End();
}

