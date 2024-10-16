#pragma once
#include "../Components/headers/GameObject.h"
#include "../Components/headers/TransformComponent.h"
#include "../Components/headers/CameraComponent.h"
#include "../Components/headers/InputComponent.h"
#include "../Components/headers/ColliderComponent.h"
#include "iostream"

class Player : public GameObject {
public:
    Player();
    TransformComponent *transform;
    CameraComponent *camera;
    ColliderComponent *collider;
    InputComponent *input;

    void start() override;
    void update(float deltaTime) override;

};

