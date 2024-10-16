#pragma once
#include "Component.h"
#include <glm.hpp>

class InputComponent : public Component {
public:
    enum Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    void processKeyboard(Movement direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset);

    // Getters
    const glm::vec3& getMovement() const;
    float getMouseX() const;
    float getMouseY() const;
    float getMoveSpeed() const;

    // Setters
    void setMovementSpeed(float speed);
    void setMouseSensitivity(float sensitivity);
    void setJumpStrength(float strength);

    void reset();

private:
    glm::vec3 m_movement{ 0.0f };
    float m_mouseX = 0.0f;
    float m_mouseY = 0.0f;
    float m_movementSpeed = 2.5f;
    float m_jumpStrength = 10.0f;
    float m_mouseSensitivity = 0.1f;
};
