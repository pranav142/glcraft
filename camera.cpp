//
// Created by pknadimp on 3/13/25.
//

#include "camera.h"
#include <glm/ext.hpp>

#include <cmath>

glm::vec3 Camera::position() const {
    return m_position;
}

void Camera::set_position(const glm::vec3 &position) {
    m_position = position;
}

glm::mat4 Camera::view_matrix() const {
    return glm::lookAt(m_position, m_front + m_position, m_up);
}

void Camera::move(CameraDirection direction, float displacement) {
    auto new_position = glm::vec3(0.0f, 0.0f, 0.0f);

    if (direction == CameraDirection::FORWARD)
        new_position += m_front * displacement;
    if (direction == CameraDirection::BACKWARD)
        new_position -= m_front * displacement;
    if (direction == CameraDirection::LEFT)
        new_position -= m_right * displacement;
    if (direction == CameraDirection::RIGHT)
        new_position += m_right * displacement;

    if (m_flying_enabled) {
        if (direction == CameraDirection::UP)
            new_position += m_world_up * displacement;
        if (direction == CameraDirection::DOWN)
            new_position -= m_world_up * displacement;
    }

    if (!m_flying_enabled) {
        new_position.y = 0;
    }

    m_position += new_position;

    update_vectors();
}

void Camera::rotate(double pitch, double yaw) {
    m_pitch += pitch;
    m_yaw += yaw;

    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    if (m_pitch < -89.0f)
        m_pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    m_front = glm::normalize(direction);

    update_vectors();
}

void Camera::enable_flying() {
    m_flying_enabled = true;
}

void Camera::disable_flying() {
    m_flying_enabled = false;
}

void Camera::update_vectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    m_right = glm::normalize(glm::cross(m_front, m_world_up));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
