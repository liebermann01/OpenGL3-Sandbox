#pragma once

#include "Scene.h"
#include "../Models/Shapes/Cube.h"
#include "../UI/Grid.h"
#include <memory>


class EditorScene : public Scene {
public:
    std::string selected_type = "";
    void setup() override;
    void update(float dt) override;
};