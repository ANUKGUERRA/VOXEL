#pragma once
#include "Component.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
class CameraComponent : public Component {
public:
    CameraComponent(const glm::vec3& position = glm::vec3(0.0f, 100.0f, 0.0f),
        const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f, float pitch = 0.0f)
        : m_position(position),
        m_worldUp(up),
        m_yaw(yaw),
        m_pitch(pitch),
        m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
        m_zoom(45.0f)
    {
        updateCameraVectors();
    }

    glm::mat4 getViewMatrix() const {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    void update(const glm::vec3& movement, float mouseX, float mouseY) {
        m_position += m_right * movement.x + m_up * movement.y + m_front * movement.z;

        m_yaw += mouseX;
        m_pitch += mouseY;

        if (m_pitch > 89.0f) m_pitch = 89.0f;
        if (m_pitch < -89.0f) m_pitch = -89.0f;

        updateCameraVectors();
    }

    // Getters
    const glm::vec3& getPosition() const { return m_position; }
    const glm::vec3& getFront() const { return m_front; }
    float getZoom() const { return m_zoom; }

private:
    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        front.y = sin(glm::radians(m_pitch));
        front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_front = glm::normalize(front);
        m_right = glm::normalize(glm::cross(m_front, m_worldUp));
        m_up = glm::normalize(glm::cross(m_right, m_front));
    }

    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;

    float m_yaw;
    float m_pitch;
    float m_zoom;
};


