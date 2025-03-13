//
// Created by pknadimp on 3/13/25.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

enum class CameraDirection {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

class Camera {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = 0.0f, float pitch = 0.0f, bool enable_flying = false) : m_position(position),
                                                                       m_up(up),
                                                                       m_yaw(yaw),
                                                                       m_pitch(pitch),
                                                                       m_flying_enabled(enable_flying) {
        update_vectors();
    };

    [[nodiscard]] glm::vec3 position() const;

    void set_position(const glm::vec3 &position);

    [[nodiscard]] glm::mat4 view_matrix() const;

    void move(CameraDirection direction, float displacement);

    void rotate(double pitch, double yaw);

    void enable_flying();

    void disable_flying();

private:
    void update_vectors();

private:
    glm::vec3 m_position = glm::vec3(0, 0, 0);
    glm::vec3 m_front = glm::vec3(0, 0, -1);;
    glm::vec3 m_up = glm::vec3(0, 1, 0);
    glm::vec3 m_right = glm::vec3(1, 0, 0);
    glm::vec3 m_world_up = glm::vec3(0, 1, 0);

    bool m_flying_enabled = false;

    double m_yaw = 0;
    double m_pitch = 0;
};


#endif //CAMERA_H
