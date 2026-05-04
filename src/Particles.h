#pragma once

#include "Shader.h"
#include "TextureAtlas.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <vector>
#include <string>

enum class BlendMode {
    Alpha = 0,
    Additive = 1,
};

struct Particle {
    glm::vec3 pos{0.0f};
    glm::vec3 vel{0.0f};
    glm::vec3 acc{0.0f};

    glm::vec4 color{1.0f};
    float size{0.1f};

    float life{0.0f};
    float max_life{1.0f};

    float rotation{0.0f};
    float tex_index{0.0f}; // stored as float to keep attrib setup simple
    bool alive{false};
};

struct EmitterSettings {
    std::string name{"fire"};

    BlendMode blend{BlendMode::Additive};

    float spawn_rate{600.0f};  // particles per second
    int burst_count{0};

    glm::vec3 position{0.0f, 0.1f, 0.0f};
    glm::vec3 position_jitter{0.15f, 0.0f, 0.15f};

    glm::vec3 velocity{0.0f, 2.0f, 0.0f};
    glm::vec3 velocity_jitter{1.2f, 0.9f, 1.2f};

    glm::vec3 acceleration{0.0f, 0.0f, 0.0f};
    glm::vec3 wind{0.0f};

    float damping{0.05f};

    glm::vec4 color_start{1.0f, 0.6f, 0.2f, 0.9f};
    glm::vec4 color_end{0.9f, 0.15f, 0.05f, 0.0f};

    float size_start{0.12f};
    float size_end{0.42f};

    float life_min{0.5f};
    float life_max{1.1f};

    int texture_tile{0}; // 0 fire 1 smoke 2 spark 3 rain 4 magic 5 soft

    bool depth_sort{true};
    bool align_to_velocity{false};

    // hdr brightness multiplier  values above 1 push the particle into bloom
    float emissive{1.0f};

    // curl noise turbulence (bridson 2007)
    // strength 0 disables it
    float curl_strength{0.0f};
    float curl_scale{0.6f};      // spatial frequency of the curl field
    float curl_time_scale{0.5f}; // how fast the field evolves over time

    // soft particle fade distance in view space units  0 disables it
    float soft_fade{0.0f};

    // velocity aligned billboard stretch  0 disables it
    float velocity_stretch{0.0f};
};

class ParticleSystem {
public:
    ParticleSystem() = default;

    void init(int max_particles = 50000);
    void shutdown();

    void set_preset(const EmitterSettings& preset);
    EmitterSettings& preset_mut() { return m_settings; }
    const EmitterSettings& preset() const { return m_settings; }

    void request_burst(int count);

    void update(float dt, const Camera& cam);

    // scene_depth and viewport are needed for soft particles
    void render(const Camera& cam, unsigned scene_depth, int viewport_w, int viewport_h);

    int alive_count() const { return m_alive_count; }
    int max_count() const { return (int)m_particles.size(); }

private:
    void spawn_one();
    float rand01();
    glm::vec3 rand_vec3(const glm::vec3& jitter);
    float rand_range(float a, float b);

    void update_instance_buffer(const Camera& cam);

private:
    EmitterSettings m_settings{};
    std::vector<Particle> m_particles;
    int m_alive_count{0};

    int m_next_free{0};
    float m_spawn_accum{0.0f};
    float m_sim_time{0.0f};

    Shader m_particle_shader;
    TextureAtlas m_atlas;

    unsigned m_quad_vao{0};
    unsigned m_quad_vbo{0};
    unsigned m_instance_vbo{0};

    struct InstanceData {
        glm::vec3 pos;
        float size;
        glm::vec4 color;
        float rotation;
        float tex_index;
        glm::vec3 vel;
    };
    std::vector<InstanceData> m_instances;

    BlendMode m_active_blend{BlendMode::Alpha};
};
