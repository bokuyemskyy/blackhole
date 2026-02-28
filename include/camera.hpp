#pragma once

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>

class Camera {
public:
    Camera(glm::vec3 position, glm::vec3 rotation, float speed, float lerp_factor = 0.1f, float sensitivity = 0.1f,
           float fov = 70.0f,
           float near_plane = 0.1f,
           float far_plane = 1000.0f);

    void processInput(GLFWwindow *window, double delta_time);

    [[nodiscard]] glm::mat4 getViewMatrix() const;

    [[nodiscard]] glm::mat4 getProjectionMatrix(float aspect) const;

    [[nodiscard]] glm::mat4 getInvViewProj(float aspect) const;

    [[nodiscard]] glm::vec3 getPosition() const;

private:
    glm::vec3 m_position;
    glm::vec3 m_velocity{};
    glm::vec3 m_rotation;

    float m_sensitivity;
    double m_last_mouse_x = 0.0;
    double m_last_mouse_y = 0.0;
    bool m_first_mouse_pos = true;

    float m_speed;
    float m_lerp_factor;
    float m_fov;
    float m_near_plane;
    float m_far_plane;
};
