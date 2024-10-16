#include "headers/InputComponent.h"
#include "iostream"
#include "headers/CameraComponent.h"

void InputComponent::processMovement(Movement direction, float deltaTime) {
    glm::vec3 front = this->gameObject->getComponent<CameraComponent>()->getFront();
    glm::vec3 right = this->gameObject->getComponent<CameraComponent>()->getRight();

    float velocity = m_movementSpeed * deltaTime;
    switch (direction) {
    case FORWARD:  m_movement += front * velocity; break;
    case BACKWARD: m_movement -= front * velocity; break;
    case LEFT:     m_movement -= right * velocity; break;
    case RIGHT:    m_movement += right * velocity; break;
    case UP:       m_movement += m_jumpStrength * deltaTime; break;
    case DOWN:     m_movement -= velocity; break;
    }
}

void InputComponent::processMouseMovement(float xoffset, float yoffset) {
    m_mouseX += xoffset * m_mouseSensitivity;
    m_mouseY += yoffset * m_mouseSensitivity;
}

const glm::vec3& InputComponent::getMovement() const {
    return m_movement;
}

float InputComponent::getMouseX() const {
    return m_mouseX;
}

float InputComponent::getMouseY() const {
    return m_mouseY;
}

float InputComponent::getMoveSpeed() const {
    return m_movementSpeed;
}

void InputComponent::setMovementSpeed(float speed) {
    m_movementSpeed = speed;
}

void InputComponent::setMouseSensitivity(float sensitivity) {
    m_mouseSensitivity = sensitivity;
}

void InputComponent::setJumpStrength(float strength) {
    m_jumpStrength = strength;
}

void InputComponent::reset() {
    m_movement = glm::vec3(0.0f);
    m_mouseX = 0.0f;
    m_mouseY = 0.0f;
}
