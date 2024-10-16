#include "Player.h"

Player::Player() {

    transform = addComponent<TransformComponent>(glm::vec3{ 0.0f,50.0f,0.0f });
    camera = addComponent<CameraComponent>(transform->position);
    collider = addComponent<ColliderComponent>(transform->position - glm::vec3{ 0.5f,1.0f,0.5f }, transform->position + glm::vec3{ 0.5f,1.0f,0.5f });
    input = addComponent<InputComponent>();
}


void Player::start() {

}
void Player::update(float deltaTime){
    transform->position += input->getMovement();
    camera->update(transform->position, input->getMouseX(), input->getMouseY());
    input->reset();
}
