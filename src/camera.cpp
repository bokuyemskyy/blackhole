#include "camera.hpp"

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 rotation, float speed, float lerp_factor, float sensitivity, float fov,
               float near_plane,
               float far_plane) : m_position(position),
                                  m_rotation(rotation), m_speed(speed), m_lerp_factor(lerp_factor),
                                  m_sensitivity(sensitivity),
                                  m_fov(fov), m_near_plane(near_plane), m_far_plane(far_plane) {
}

void Camera::processInput(GLFWwindow *window, double delta_time) {
    glm::vec3 targetVelocity(0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) targetVelocity.z += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) targetVelocity.z -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) targetVelocity.x += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) targetVelocity.x -= 1.0f;

    if (glm::length(targetVelocity) > 0.0f)
        targetVelocity = glm::normalize(targetVelocity) * m_speed;

    glm::vec3 forward;
    forward.x = cos(glm::radians(m_rotation.y)) * cos(glm::radians(m_rotation.x));
    forward.y = sin(glm::radians(m_rotation.x));
    forward.z = sin(glm::radians(m_rotation.y)) * cos(glm::radians(m_rotation.x));
    forward = glm::normalize(forward);

    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));

    glm::vec3 worldVelocity = targetVelocity.x * right + targetVelocity.z * forward;

    m_velocity = glm::mix(m_velocity, worldVelocity, m_lerp_factor);

    m_position += m_velocity * static_cast<float>(delta_time);

    double mouse_x, mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    if (m_first_mouse_pos) {
        m_last_mouse_x = mouse_x;
        m_last_mouse_y = mouse_y;
        m_first_mouse_pos = false;
    }

    float offset_x = mouse_x - m_last_mouse_x;
    float offset_y = m_last_mouse_y - mouse_y;

    m_last_mouse_x = mouse_x;
    m_last_mouse_y = mouse_y;

    m_rotation.y += offset_x * m_sensitivity;
    m_rotation.x += offset_y * m_sensitivity;

    m_rotation.x = glm::clamp(m_rotation.x, -89.0f, 89.0f);
}


glm::mat4 Camera::getViewMatrix() const {
    glm::vec3 forward;
    forward.x = cos(glm::radians(m_rotation.y)) * cos(glm::radians(m_rotation.x));
    forward.y = sin(glm::radians(m_rotation.x));
    forward.z = sin(glm::radians(m_rotation.y)) * cos(glm::radians(m_rotation.x));
    forward = glm::normalize(forward);

    glm::vec3 up(0.0f, 1.0f, 0.0f);

    return glm::lookAt(m_position, m_position + forward, up);
}

glm::mat4 Camera::getProjectionMatrix(float aspect) const {
    return
            glm::perspective(glm::radians
                             (m_fov), aspect, m_near_plane, m_far_plane
            );
}

glm::mat4 Camera::getInvViewProj(float aspect) const {
    glm::mat4 proj = getProjectionMatrix(aspect);
    glm::mat4 view = getViewMatrix();
    return glm::inverse(proj * view);
}

glm::vec3 Camera::getPosition() const {
    return m_position;
}
