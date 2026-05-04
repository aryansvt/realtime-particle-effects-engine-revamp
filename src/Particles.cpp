#include "Particles.h"
#include "CurlNoise.h"

#include <glad/glad.h>
#include <algorithm>
#include <random>
#include <cmath>

static uint32_t wang_hash(uint32_t seed) {
    seed = (seed ^ 61u) ^ (seed >> 16u);
    seed *= 9u;
    seed = seed ^ (seed >> 4u);
    seed *= 0x27d4eb2du;
    seed = seed ^ (seed >> 15u);
    return seed;
}

static float hash01(uint32_t& state) {
    state = wang_hash(state);
    return (state & 0x00FFFFFF) / 16777216.0f;
}



void ParticleSystem::init(int max_particles) {
    m_particles.resize(max_particles);
    m_instances.reserve(max_particles);

    // quad vertices geometry
    const float quad[] = {
        // corner.xy   uv.xy
        -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  1.0f, 1.0f,

        -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.0f, 1.0f,
    };

    glGenVertexArrays(1, &m_quad_vao);
    glBindVertexArray(m_quad_vao);

    glGenBuffers(1, &m_quad_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, (ptrdiff_t)sizeof(quad), quad, GL_STATIC_DRAW);

    // quad vertices
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    // per particle instance buffer
    glGenBuffers(1, &m_instance_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_instance_vbo);
    glBufferData(GL_ARRAY_BUFFER, (ptrdiff_t)(sizeof(InstanceData) * max_particles), nullptr, GL_STREAM_DRAW);

    // instance attributes
    const size_t stride = sizeof(InstanceData);

    glEnableVertexAttribArray(2); // pos
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, (GLsizei)stride, (void*)offsetof(InstanceData, pos));
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3); // size
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, (GLsizei)stride, (void*)offsetof(InstanceData, size));
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(4); // color
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, (GLsizei)stride, (void*)offsetof(InstanceData, color));
    glVertexAttribDivisor(4, 1);

    glEnableVertexAttribArray(5); // rotation
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, (GLsizei)stride, (void*)offsetof(InstanceData, rotation));
    glVertexAttribDivisor(5, 1);

    glEnableVertexAttribArray(6); // tile index packed as float
    glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, (GLsizei)stride, (void*)offsetof(InstanceData, tex_index));
    glVertexAttribDivisor(6, 1);

    glEnableVertexAttribArray(7); // velocity for stretched billboards
    glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, (GLsizei)stride, (void*)offsetof(InstanceData, vel));
    glVertexAttribDivisor(7, 1);

    glBindVertexArray(0);

    m_particle_shader.load("shaders/particle.vert", "shaders/particle.frag");

    m_atlas.build_default_atlas();

    set_preset(m_settings);
}

void ParticleSystem::shutdown() {
    m_particle_shader.destroy();
    m_atlas.destroy();

    if (m_instance_vbo) glDeleteBuffers(1, &m_instance_vbo);
    if (m_quad_vbo) glDeleteBuffers(1, &m_quad_vbo);
    if (m_quad_vao) glDeleteVertexArrays(1, &m_quad_vao);

    m_instance_vbo = 0;
    m_quad_vbo = 0;
    m_quad_vao = 0;
}

void ParticleSystem::set_preset(const EmitterSettings& preset) {
    m_settings = preset;
    m_active_blend = m_settings.blend;

    // reset internal spawn accumulator
    m_spawn_accum = 0.0f;

    // soft reset but dont nuke if you want to blend presets quickly
    for (auto& p : m_particles) p.alive = false;
    m_alive_count = 0;
    m_next_free = 0;
}

void ParticleSystem::request_burst(int count) {
    m_settings.burst_count += count;
}

float ParticleSystem::rand01() {
    static uint32_t s = 0x12345678u;
    return hash01(s);
}

float ParticleSystem::rand_range(float a, float b) {
    return a + (b - a) * rand01();
}

glm::vec3 ParticleSystem::rand_vec3(const glm::vec3& jitter) {
    return glm::vec3(
        rand_range(-jitter.x, jitter.x),
        rand_range(-jitter.y, jitter.y),
        rand_range(-jitter.z, jitter.z)
    );
}

void ParticleSystem::spawn_one() {
    // find a free slot in the pool
    int tries = 0;
    while (tries < (int)m_particles.size() && m_particles[m_next_free].alive) {
        m_next_free = (m_next_free + 1) % (int)m_particles.size();
        tries++;
    }
    if (tries >= (int)m_particles.size()) return; // pool is full

    Particle& p = m_particles[m_next_free];
    p.alive = true;

    p.pos = m_settings.position + rand_vec3(m_settings.position_jitter);
    p.vel = m_settings.velocity + rand_vec3(m_settings.velocity_jitter);
    p.acc = m_settings.acceleration;

    p.max_life = rand_range(m_settings.life_min, m_settings.life_max);
    p.life = p.max_life;

    p.size = m_settings.size_start;
    p.color = m_settings.color_start;

    p.rotation = rand_range(0.0f, 6.28318f);
    p.tex_index = (float)m_settings.texture_tile;

    m_alive_count++;
    m_next_free = (m_next_free + 1) % (int)m_particles.size();
}

void ParticleSystem::update(float dt, const Camera& cam) {
    m_sim_time += dt;

    // spawn based on rate
    m_spawn_accum += m_settings.spawn_rate * dt;
    while (m_spawn_accum >= 1.0f) {
        spawn_one();
        m_spawn_accum -= 1.0f;
    }

    // handle bursts
    while (m_settings.burst_count > 0) {
        spawn_one();
        m_settings.burst_count--;
    }

    const bool use_curl = m_settings.curl_strength > 0.0001f;
    const float curl_t = m_sim_time * m_settings.curl_time_scale;

    // update particle state
    m_alive_count = 0;
    for (auto& p : m_particles) {
        if (!p.alive) continue;

        // base acceleration plus wind
        glm::vec3 acc = p.acc + m_settings.wind;

        // curl noise turbulence applied as a velocity perturbation
        // the field is divergence free so particles curl rather than compress
        if (use_curl) {
            glm::vec3 c = curl_noise::curl(p.pos, m_settings.curl_scale, curl_t);
            acc += c * m_settings.curl_strength;
        }

        p.vel += acc * dt;
        p.vel *= (1.0f - (std::min)(0.99f, m_settings.damping * dt));
        p.pos += p.vel * dt;

        p.life -= dt;
        if (p.life <= 0.0f) {
            p.alive = false;
            continue;
        }

        // normalized age
        float t = 1.0f - (p.life / p.max_life);

        // size and color over lifetime
        p.size = glm::mix(m_settings.size_start, m_settings.size_end, t);
        p.color = glm::mix(m_settings.color_start, m_settings.color_end, t);

        if (m_settings.align_to_velocity) {
            // optionally align rotation to velocity in screen space
            // separate from velocity stretch which actually deforms the quad
            glm::vec3 v = p.vel;
            if (glm::length(v) > 0.0001f) {
                float x = glm::dot(glm::normalize(v), cam.right());
                float y = glm::dot(glm::normalize(v), cam.up());
                p.rotation = std::atan2(y, x);
            }
        }

        m_alive_count++;
    }

    update_instance_buffer(cam);
}

void ParticleSystem::update_instance_buffer(const Camera& cam) {
    m_instances.clear();
    m_instances.reserve((size_t)m_alive_count);

    // gather alive particles into instances
    // bake the emissive multiplier into the rgb channel so hdr bloom can pick it up
    const float emi = m_settings.emissive;
    for (const auto& p : m_particles) {
        if (!p.alive) continue;
        InstanceData d{};
        d.pos = p.pos;
        d.size = p.size;
        d.color = glm::vec4(p.color.r * emi, p.color.g * emi, p.color.b * emi, p.color.a);
        d.rotation = p.rotation;
        d.tex_index = p.tex_index;
        d.vel = p.vel;
        m_instances.push_back(d);
    }

    // depth sort for alpha blending
    if (m_settings.depth_sort && m_active_blend == BlendMode::Alpha) {
        const glm::vec3 cam_pos = cam.position();
        std::sort(m_instances.begin(), m_instances.end(), [&](const InstanceData& a, const InstanceData& b) {
            float da = glm::dot(a.pos - cam_pos, a.pos - cam_pos);
            float db = glm::dot(b.pos - cam_pos, b.pos - cam_pos);
            return da > db; // back to front order
        });
    }

    // upload the instance buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_instance_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, (ptrdiff_t)(m_instances.size() * sizeof(InstanceData)), m_instances.data());
}

void ParticleSystem::render(const Camera& cam, unsigned scene_depth, int viewport_w, int viewport_h) {
    if (m_instances.empty()) return;

    // set the blend mode
    glEnable(GL_BLEND);
    if (m_active_blend == BlendMode::Additive) {
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    } else {
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    glDepthMask(GL_FALSE); // dont write depth for sprites
    glDisable(GL_CULL_FACE);

    m_particle_shader.use();
    m_particle_shader.set_mat4("uView", cam.view());
    m_particle_shader.set_mat4("uProj", cam.projection());
    m_particle_shader.set_vec3("uCamRight", cam.right());
    m_particle_shader.set_vec3("uCamUp", cam.up());
    m_particle_shader.set_vec3("uCamForward", cam.forward());
    m_particle_shader.set_vec2("uTileScale", m_atlas.tile_uv_scale());
    m_particle_shader.set_float("uVelStretch", m_settings.velocity_stretch);
    m_particle_shader.set_float("uSoftFade", m_settings.soft_fade);
    m_particle_shader.set_vec2("uViewport", glm::vec2((float)viewport_w, (float)viewport_h));
    m_particle_shader.set_vec2("uDepthRange", glm::vec2(cam.near_plane(), cam.far_plane()));

    m_particle_shader.set_int("uAtlas", 0);
    m_particle_shader.set_int("uSceneDepth", 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_atlas.id());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, scene_depth);

    glBindVertexArray(m_quad_vao);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, (GLsizei)m_instances.size());
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
}
