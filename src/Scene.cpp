#include "Scene.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

void Scene::init() {
    // ground plane quad
    const float verts[] = {
        // position
        -50.0f, 0.0f, -50.0f,
         50.0f, 0.0f, -50.0f,
         50.0f, 0.0f,  50.0f,
        -50.0f, 0.0f, -50.0f,
         50.0f, 0.0f,  50.0f,
        -50.0f, 0.0f,  50.0f,
    };

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, (ptrdiff_t)sizeof(verts), verts, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);

    m_ground_shader.load("shaders/ground.vert", "shaders/ground.frag");
}

void Scene::shutdown() {
    m_ground_shader.destroy();
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    m_vbo = 0;
    m_vao = 0;
}

void Scene::render(const glm::mat4& view, const glm::mat4& proj) {
    m_ground_shader.use();
    m_ground_shader.set_mat4("uView", view);
    m_ground_shader.set_mat4("uProj", proj);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
