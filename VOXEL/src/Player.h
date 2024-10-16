#pragma once
#include "GameObject/headers/GameObject.h"
#include "GameObject/headers/TransformComponent.h"
#include "GameObject/headers/CameraComponent.h"
#include "GameObject/headers/InputComponent.h"
#include "GameObject/headers/ColliderComponent.h"
#include "iostream"

class Player : public GameObject {
public:
    Player() {
        
        transform = addComponent<TransformComponent>(glm::vec3{ 0.0f,50.0f,0.0f });
        camera = addComponent<CameraComponent>(transform->position);
        collider = addComponent<ColliderComponent>(transform->position - glm::vec3{0.5f,1.0f,0.5f}, transform->position + glm::vec3{ 0.5f,1.0f,0.5f });
        input = addComponent<InputComponent>();
    }
    TransformComponent *transform;
    CameraComponent *camera;
    ColliderComponent *collider;
    InputComponent *input;

    void update(float deltaTime) override {
        camera->update(transform->position, input->getMouseX(), input->getMouseY());
    }
};

