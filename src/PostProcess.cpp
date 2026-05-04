#include "PostProcess.h"

#include <glad/glad.h>
#include <iostream>
#include <algorithm>

void PostProcess::init(int width, int height) {
    m_width = width;
    m_height = height;
    m_bloom_w = (std::max)(1, width / 2);
    m_bloom_h = (std::max)(1, height / 2);

    // empty vao required by core profile even for vertex pulled draws
    glGenVertexArrays(1, &m_empty_vao);

    create_hdr_fbo();
    create_bloom_fbos();

    m_threshold.load("shaders/fullscreen.vert", "shaders/bloom_threshold.frag");
    m_blur.load("shaders/fullscreen.vert", "shaders/bloom_blur.frag");
    m_composite.load("shaders/fullscreen.vert", "shaders/composite.frag");
}

void PostProcess::resize(int width, int height) {
    if (width == m_width && height == m_height) return;
    m_width = width;
    m_height = height;
    m_bloom_w = (std::max)(1, width / 2);
    m_bloom_h = (std::max)(1, height / 2);

    destroy_fbos();
    create_hdr_fbo();
    create_bloom_fbos();
}

void PostProcess::shutdown() {
    destroy_fbos();

    m_threshold.destroy();
    m_blur.destroy();
    m_composite.destroy();

    if (m_empty_vao) glDeleteVertexArrays(1, &m_empty_vao);
    m_empty_vao = 0;
}

void PostProcess::create_hdr_fbo() {
    // hdr color  rgba16f for high range without going to full float
    glGenTextures(1, &m_hdr_color);
    glBindTexture(GL_TEXTURE_2D, m_hdr_color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // depth as a sampleable texture so particles can read it for soft particle fade
    glGenTextures(1, &m_hdr_depth);
    glBindTexture(GL_TEXTURE_2D, m_hdr_depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenFramebuffers(1, &m_hdr_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_hdr_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_hdr_color, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_hdr_depth, 0);

    GLenum bufs[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, bufs);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "hdr fbo incomplete\n";
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcess::create_bloom_fbos() {
    glGenFramebuffers(2, m_bloom_fbo);
    glGenTextures(2, m_bloom_tex);

    for (int i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, m_bloom_tex[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_bloom_w, m_bloom_h, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindFramebuffer(GL_FRAMEBUFFER, m_bloom_fbo[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_bloom_tex[i], 0);

        GLenum bufs[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, bufs);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "bloom fbo " << i << " incomplete\n";
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcess::destroy_fbos() {
    if (m_hdr_fbo)   { glDeleteFramebuffers(1, &m_hdr_fbo);   m_hdr_fbo = 0; }
    if (m_hdr_color) { glDeleteTextures(1, &m_hdr_color);     m_hdr_color = 0; }
    if (m_hdr_depth) { glDeleteTextures(1, &m_hdr_depth);     m_hdr_depth = 0; }

    if (m_bloom_fbo[0] || m_bloom_fbo[1]) {
        glDeleteFramebuffers(2, m_bloom_fbo);
        m_bloom_fbo[0] = m_bloom_fbo[1] = 0;
    }
    if (m_bloom_tex[0] || m_bloom_tex[1]) {
        glDeleteTextures(2, m_bloom_tex);
        m_bloom_tex[0] = m_bloom_tex[1] = 0;
    }
}

void PostProcess::draw_fullscreen() {
    glBindVertexArray(m_empty_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void PostProcess::begin_scene_pass() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_hdr_fbo);
    glViewport(0, 0, m_width, m_height);
    glClearColor(0.05f, 0.06f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PostProcess::end_and_present(int screen_w, int screen_h) {
    // we are about to read attachments via samplers so disable depth and blend
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    // 1 threshold pass writes bright pixels into bloom_fbo[0]
    if (bloom_enabled) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_bloom_fbo[0]);
        glViewport(0, 0, m_bloom_w, m_bloom_h);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_threshold.use();
        m_threshold.set_int("uScene", 0);
        m_threshold.set_float("uThreshold", bloom_threshold);
        m_threshold.set_float("uKnee", 0.5f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_hdr_color);
        draw_fullscreen();

        // 2 separable gaussian ping pong
        m_blur.use();
        m_blur.set_int("uTex", 0);
        glm::vec2 texel(1.0f / (float)m_bloom_w, 1.0f / (float)m_bloom_h);
        m_blur.set_vec2("uTexel", texel);

        bool horizontal = true;
        // each iteration is a horizontal then vertical pass
        for (int i = 0; i < blur_iterations * 2; i++) {
            int dst = horizontal ? 1 : 0;
            int src = horizontal ? 0 : 1;
            glBindFramebuffer(GL_FRAMEBUFFER, m_bloom_fbo[dst]);
            m_blur.set_int("uHorizontal", horizontal ? 1 : 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_bloom_tex[src]);
            draw_fullscreen();
            horizontal = !horizontal;
        }
    }

    // 3 composite to screen with tonemap and gamma
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, screen_w, screen_h);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_composite.use();
    m_composite.set_int("uScene", 0);
    m_composite.set_int("uBloom", 1);
    m_composite.set_float("uBloomStrength", bloom_enabled ? bloom_strength : 0.0f);
    m_composite.set_float("uExposure", exposure);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_hdr_color);
    glActiveTexture(GL_TEXTURE1);
    // when bloom is off we still bind something safe
    glBindTexture(GL_TEXTURE_2D, bloom_enabled ? m_bloom_tex[0] : m_hdr_color);

    draw_fullscreen();

    // restore depth and blend for next frame
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
}
