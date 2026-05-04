#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(float aspect);

    void set_aspect(float aspect);
    void set_position(const glm::vec3& p);

    void on_mouse_delta(float dx, float dy);
    void move_local(const glm::vec3& local_dir, float dt);

    glm::mat4 view() const;
    glm::mat4 projection() const;

    glm::vec3 position() const { return m_position; }
    glm::vec3 forward() const;
    glm::vec3 right() const;
    glm::vec3 up() const;

    float yaw() const { return m_yaw; }
    float pitch() const { return m_pitch; }

    float near_plane() const { return m_near; }
    float far_plane() const { return m_far; }

    float move_speed{6.0f};
    float mouse_sensitivity{0.10f};

private:
    glm::vec3 m_position{0.0f, 1.6f, 6.0f};
    float m_yaw{-90.0f};
    float m_pitch{0.0f};
    float m_fov{60.0f};
    float m_aspect{16.0f/9.0f};
    float m_near{0.05f};
    float m_far{250.0f};
};
