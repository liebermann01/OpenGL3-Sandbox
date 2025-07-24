#pragma once

#include <vector>
#include <memory>
#include <string>
#include "imgui/imgui.h"
#include "../Models/Object.h"
#include "../Cameras/camera.h"


class Scene {
    int next_id = 0;
    std::vector<std::shared_ptr<Object>> objects;

public:
    bool grid_layer = true;

    void addObject(std::shared_ptr<Object> obj);
    void removeObject(int id);
    virtual void update(float dt);
    void draw(const Camera& cam);
    virtual void setup() = 0;

    std::vector<std::shared_ptr<Object>>& getObjects();
};
