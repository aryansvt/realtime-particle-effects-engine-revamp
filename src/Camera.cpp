#include "Camera.h"

static float clampf(float v, float lo, float hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

Camera::Camera(float aspect) : m_aspect(aspect) {}

void Camera::set_aspect(float aspect) {
    m_aspect = aspect;
}

void Camera::set_position(const glm::vec3& p) {
    m_position = p;
}

glm::vec3 Camera::forward() const {
    const float yaw_r = glm::radians(m_yaw);
    const float pitch_r = glm::radians(m_pitch);
    glm::vec3 f;
    f.x = cosf(yaw_r) * cosf(pitch_r);
    f.y = sinf(pitch_r);
    f.z = sinf(yaw_r) * cosf(pitch_r);
    return glm::normalize(f);
}

glm::vec3 Camera::right() const {
    return glm::normalize(glm::cross(forward(), glm::vec3(0.0f, 1.0f, 0.0f)));
}

glm::vec3 Camera::up() const {
    return glm::normalize(glm::cross(right(), forward()));
}

void Camera::on_mouse_delta(float dx, float dy) {
    m_yaw += dx * mouse_sensitivity;
    m_pitch -= dy * mouse_sensitivity;
    m_pitch = clampf(m_pitch, -89.0f, 89.0f);
}

void Camera::move_local(const glm::vec3& local_dir, float dt) {
    // local_dir is in camera space
    const glm::vec3 f = forward();
    const glm::vec3 r = right();
    const glm::vec3 u = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 delta = r * local_dir.x + u * local_dir.y + f * local_dir.z;
    if (glm::length(delta) > 0.0001f) {
        m_position += glm::normalize(delta) * move_speed * dt;
    }
}

glm::mat4 Camera::view() const {
    return glm::lookAt(m_position, m_position + forward(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::projection() const {
    return glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
}
