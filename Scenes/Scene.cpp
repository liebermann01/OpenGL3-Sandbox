#include "Scene.h"

void Scene::addObject(std::shared_ptr<Object> obj) {
    obj->setId(next_id++);
    objects.push_back(std::move(obj));
}

void Scene::removeObject(int id) {
    objects.erase(
        std::remove_if(objects.begin(), objects.end(),
            [id](const std::shared_ptr<Object>& obj) {
                return obj->getId() == id;
            }),
        objects.end());
}


void Scene::update(float dt) {
    for (auto& obj : objects) {
        obj->update(dt);
        if (obj->layer != 0 &&
            ImGui::TreeNode(("[" + std::to_string(obj->getId()) + "] " + obj->getName() + std::string("##")).c_str())) {
            obj->renderUI();
            ImGui::TreePop();
        }
    }

    objects.erase(
        std::remove_if(objects.begin(), objects.end(),
            [](const std::shared_ptr<Object>& obj) {
                return obj->isMarkedForRemoval();
            }),
        objects.end());
}

void Scene::draw(const Camera& cam) {
    for (const auto& obj : objects) {
        if (obj->active && !(obj->layer == 0 && !grid_layer))
            obj->draw(cam);
    }
}

std::vector<std::shared_ptr<Object>>& Scene::getObjects() {
    return objects;
}