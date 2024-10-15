#pragma once
#include "GameObject/headers/GameObject.h"
#include "GameObject/headers/TransformComponent.h"
#include "GameObject/headers/CameraComponent.h"
#include "GameObject/headers/InputComponent.h"
#include "GameObject/headers/ColliderComponent.h"

class Player : public GameObject {
public:
    Player() {
        addComponent<TransformComponent>();
        addComponent<CameraComponent>();
        addComponent<ColliderComponent>();
        addComponent<InputComponent>();
    }

    void update(float deltaTime) override {
    }
};

