#pragma once

#include "Shader.h"

#include <glm/glm.hpp>

class Scene {
public:
    void init();
    void shutdown();

    void render(const glm::mat4& view, const glm::mat4& proj);

private:
    Shader m_ground_shader;
    unsigned m_vao{0};
    unsigned m_vbo{0};
};
