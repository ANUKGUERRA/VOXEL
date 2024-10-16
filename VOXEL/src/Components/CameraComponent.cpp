#include "headers/CameraComponent.h"
#include <gtc/matrix_transform.hpp>
#include <iostream>

CameraComponent::CameraComponent(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch)
    : m_position(position),
    m_worldUp(up),
    m_yaw(yaw),
    m_pitch(pitch),
    m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
    m_zoom(45.0f)
{
    updateCameraVectors();
}

glm::mat4 CameraComponent::getViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void CameraComponent::update(const glm::vec3& position, float mouseX, float mouseY) {
    m_position = position;

    m_yaw += mouseX;
    m_pitch += mouseY;

    if (m_pitch > 89.0f) m_pitch = 89.0f;
    if (m_pitch < -89.0f) m_pitch = -89.0f;

    updateCameraVectors();
}

const glm::vec3& CameraComponent::getPosition() const {
    return m_position;
}

const glm::vec3& CameraComponent::getFront() const {
    return m_front;
}
const glm::vec3& CameraComponent::getRight() const {
    return m_right;
}

float CameraComponent::getZoom() const {
    return m_zoom;
}

void CameraComponent::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
