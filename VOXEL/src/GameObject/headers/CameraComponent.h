#pragma once
#include "Component.h"
#include <glm.hpp>

class CameraComponent : public Component {
public:
    CameraComponent(const glm::vec3& position = glm::vec3(0.0f, 100.0f, 0.0f),
        const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f, float pitch = 0.0f);

    glm::mat4 getViewMatrix() const;
    void update(const glm::vec3& movement, float mouseX, float mouseY);

    // Getters
    const glm::vec3& getPosition() const;
    const glm::vec3& getFront() const;
    float getZoom() const;

private:
    void updateCameraVectors();

    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;

    float m_yaw;
    float m_pitch;
    float m_zoom;
};
