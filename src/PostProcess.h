#pragma once

#include "Shader.h"

// hdr framebuffer with bloom post processing
// scene renders to a 16f color attachment plus a depth texture
// bloom is a multi pass blur of bright pixels
// final composite tonemaps hdr + bloom into the default backbuffer

class PostProcess {
public:
    void init(int width, int height);
    void resize(int width, int height);
    void shutdown();

    // start scene pass: bind hdr fbo and clear
    void begin_scene_pass();

    // end scene pass: do bloom and composite to the screen
    void end_and_present(int screen_w, int screen_h);

    // depth texture exposed so particles can do soft particle fade
    unsigned scene_depth_texture() const { return m_hdr_depth; }

    int width() const { return m_width; }
    int height() const { return m_height; }

    // tunables
    bool  bloom_enabled{true};
    float bloom_threshold{1.05f};
    float bloom_strength{0.7f};
    float exposure{1.0f};
    int   blur_iterations{5};

private:
    void create_hdr_fbo();
    void create_bloom_fbos();
    void destroy_fbos();
    void draw_fullscreen();

    int m_width{0};
    int m_height{0};

    // scene hdr target
    unsigned m_hdr_fbo{0};
    unsigned m_hdr_color{0};
    unsigned m_hdr_depth{0};

    // bloom ping pong  half resolution
    int m_bloom_w{0};
    int m_bloom_h{0};
    unsigned m_bloom_fbo[2]{0, 0};
    unsigned m_bloom_tex[2]{0, 0};

    // shaders
    Shader m_threshold;
    Shader m_blur;
    Shader m_composite;

    // empty vao for fullscreen triangle draw
    unsigned m_empty_vao{0};
};
