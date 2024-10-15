#pragma once
#include "Component.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
class InputComponent : public Component{
public:
    enum Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    void processKeyboard(Movement direction, float deltaTime) {
        float velocity = m_movementSpeed * deltaTime;
        switch (direction) {
        case FORWARD:  m_movement.z -= velocity; break;
        case BACKWARD: m_movement.z += velocity; break;
        case LEFT:     m_movement.x -= velocity; break;
        case RIGHT:    m_movement.x += velocity; break;
        case UP:       m_movement.y += m_jumpStrength * deltaTime; break;
        case DOWN:     m_movement.y -= velocity; break;
        }
    }

    void processMouseMovement(float xoffset, float yoffset) {
        m_mouseX += xoffset * m_mouseSensitivity;
        m_mouseY += yoffset * m_mouseSensitivity;
    }

    // Getters
    const glm::vec3& getMovement() const { return m_movement; }
    float getMouseX() const { return m_mouseX; }
    float getMouseY() const { return m_mouseY; }
    float getMoveSpeed() const { return m_movementSpeed; }

    // Setters
    void setMovementSpeed(float speed) { m_movementSpeed = speed; }
    void setMouseSensitivity(float sensitivity) { m_mouseSensitivity = sensitivity; }
    void setJumpStrength(float strength) { m_jumpStrength = strength; }

    void reset() {
        m_movement = glm::vec3(0.0f);
        m_mouseX = 0.0f;
        m_mouseY = 0.0f;
    }

private:
    glm::vec3 m_movement{ 0.0f };
    float m_mouseX = 0.0f;
    float m_mouseY = 0.0f;
    float m_movementSpeed = 2.5f;
    float m_jumpStrength = 10.0f;
    float m_mouseSensitivity = 0.1f;
};


